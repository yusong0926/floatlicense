/**
 * Created by syu on 8/30/16.
 */


var checkout = function(key, licenses, socket) {

    if(key in licenses) {

        var info = licenses[key];
        if(info.reserved_num < info.purchased_num) {
            info.reserved_num++;
            console.log("checkout ok, key: " + key + "reserved: " + info.reserved_num + "purchased: " + info.purchased_num);
            socket.write('accept\r\n');
        } else {
            console.log("ran out of licenses, key: " + key + "reserved: " + info.reserved_num + "purchased: " + info.purchased_num);
            socket.write('reject\r\n');
            socket.end("ran out of licenses\r\n");
        }

    } else {
        console.log("unknown key: ", key);
        socket.end("unknown\r\n");
    }
};


var release= function(key, licenses, socket) {

    if(key in licenses) {

        var info = licenses[key];

        if(info.reserved_num > 0) {
            info.reserved_num--;
            console.log("release ok, key: " + key + "reserved: " + info.reserved_num + "purchased: " + info.purchased_num);
        }
        socket.end("release\r\n");
    } else {
        console.log("unknown key: ", key);
        socket.end("unknown\r\n");
        socket.destroy();
    }

};

var auto_release= function(key, licenses, socket) {

    if(key in licenses) {
        var info = licenses[key];
        if(info.reserved_num > 0) {
            info.reserved_num--;
            console.log("release key automactically, key: " + key + "reserved: " + info.reserved_num + "purchased: " + info.purchased_num);
        }
    } else {
        console.log("unknown key: ", key);
    }

};


module.exports = {
    checkout: checkout,
    release: release,
    auto_release : auto_release
};