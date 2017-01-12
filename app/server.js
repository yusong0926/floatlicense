/**
 * Created by syu on 8/29/16.
 */

var net = require('net');

var server = net.createServer(function(socket) {

    var key_reserved = false;
    var key_exist = false;
    var theKey = "";
    var exit_state = "DEFAULT";

    socket.setTimeout(10000, function(){
        exit_state = "INACTIVE_SOCKET";
        socket.end();
    });
    var connection = mysql.createConnection({
        host     : "localhost",
        user     : 'syu',
        password : '6209876',
        database : 'LICENSE_DB'
    });

    connection.connect();


    socket.on('data', function (data) {
        //parse message
        var raw = data.toString('ascii').split('$');
        var header = raw[0];
        var service = raw[1];
        var key = raw[2];
        var purchased_num;
        var reserved_num;
        console.log("header is ", header);

        if(header != "uofc") {
            exit_state = "UNKNOWN_HEADER";
            socket.end("wrong header, close socket");
            return;
        }
        //hearbeat
        if (service === "alive") {
            console.log("alive");
            return;
        }

        if (service === "checkout" || service === "release") {

            console.log("raw is ", raw);
            console.log("key is ", key);
            var queryString = 'SELECT * FROM bos_licenses WHERE company = ?';
            connection.query(queryString, [key], function(err, rows) {
                if (err) {
                    console.log('rollback0, fail!', err);
                } else{
                    if (rows.length != 0) {
                        purchased_num = rows[0].purchased_num;
                        reserved_num = rows[0].reserved_num;
                        key_exist = true;
                    } else {
                        key_exist = false;
                    }
                }

                if (key_exist) {
                    theKey = key;
                    if(service === "checkout") {
                        console.log("checkout, ", purchased_num, reserved_num);
                        if (purchased_num > reserved_num) {
                            queryString = 'UPDATE bos_licenses SET reserved_num = reserved_num+1 WHERE company = ?';
                            connection.query(queryString, [key], function(err, result) {
                                if(err) {
                                    console.log('rollback1, fail!', err);
                                } else {
                                    socket.write("accept");
                                    console.log("reserve license successfully");
                                    key_reserved = true;
                                }
                            });

                        } else {
                            socket.write("reject");
                            console.log("no more license available")
                            exit_state = "LICENSE_RUN_OUT";
                            socket.end();
                            return;
                        }

                    } else if (service === "release") {
                        console.log("release key is, ", key);
                        queryString = 'UPDATE bos_licenses SET reserved_num = reserved_num-1 WHERE company = ?';
                        connection.query(queryString, [key], function(err, result) {
                            if(err) {
                                console.log('rollback4, fail!', err);
                            } else {
                                socket.write("released");
                                key_reserved = false;
                                exit_state = "LICENSE_RELEASED";
                                socket.end();
                            }
                        });
                    } else {
                        console.log("undefined operation");
                    }
                } else {
                    exit_state = "KEY_NOT_FOUND";
                    socket.end("key not found");
                    return;
                }
            });

        } else {
            exit_state = "UNKNOWN_SERVICE";
            console.log("unknow service");
        }
    });


    socket.on('close', function () {
        //console.log(exit_state);
        switch(exit_state) {
            case "UNKOWN_HEADER" :
                console.log("exit with wrong header");
                break;
            case "KEY_NOT_FOUND" :
                console.log("key not found");
                break;
            case "INACTIVE_SOCKET" :
                console.log("inactive socket");
                break;
            case "UNKNOWN_SERVICE" :
                console.log("unknown service");
                break;
            case "LICENSE_RUN_OUT" :
                console.log("ran out of licenses");
                break;
            case "LICENSE_RELEASED" :
                console.log("license released, exit normally");
                break;
            case "DEFAULT" :
                console.log("exit with socket down");
                break;
        }

        if (key_reserved) {
            var queryString = 'UPDATE bos_licenses SET reserved_num = reserved_num-1 WHERE company = ?';
            connection.query(queryString, [theKey], function(err, result) {
                if(err) {
                    console.log('rollback6, fail!', err);
                } else {
                    console.log("license released by system!");
                }
            });
        }
        connection.end();
        this.destroy();
    });
});

//server.listen(8888, '136.159.119.220');
server.listen(8888,'127.0.0.1');