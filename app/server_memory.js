/**
 * Created by syu on 8/29/16.
 */

var net = require('net');
var licenseService = require('./services/licenseService');
var dbService = require('./services/dbService');

var mongoose = require('mongoose');
mongoose.connect('mongodb://bos_admin:bos_admin@ds019906.mlab.com:19906/floatlicense');

/*
var posix = require('posix');
// raise maximum number of open file descriptors to 10k,
// hard limit is left unchanged
posix.setrlimit('nofile', { soft: 10000, hard: 10000 });
*/

var bos_licenses = {};
var wb_licenses = {};

//load licenses infos from DataBase to memory
dbService.loadData(bos_licenses, "bos");
dbService.loadData(wb_licenses, "wb");

//console.log(bos_licenses);

var server = net.createServer(function(socket) {

    var key_reserved = false;
    var theLicenses =  {};
    var theKey = "";

    socket.setTimeout(10000, function(){
        console.log("inactive socket");
        if (key_reserved) {
            licenseService.auto_release(theKey, theLicenses, socket);
            key_reserved = false;
        }
        this.destroy();
    });


    socket.on('data', function (data) {
        //parse message
        var raw = data.toString('ascii').split('$');
        var header = raw[0];
        var software = raw[1];
        var service = raw[2];
        var key = raw[3];
        var licenses={};
        if(header != "uofc") {
            console.log("unknown header: ", header);
            socket.end("unknown\r\n");
            return;
        }

        switch (software) {
            case "bos" :
                licenses = bos_licenses;
                theLicenses = licenses;
                break;
            case "wb" :
                licenses = wb_licenses;
                theLicenses = licenses;
                break;
            default :
                console.log("unknown software: ", software);
                socket.end("unkonwn software\r\n");
                socket.destroy();
                return;
        }

        switch (service) {
            case "checkout" :
                console.log("license checkout: key: ", key);
                licenseService.checkout(key, licenses, socket);
                theKey = key;
                key_reserved = true;
                break;
            case "release" :
                console.log("license release: key: ", key);
                licenseService.release(key, licenses, socket);
                key_reserved = false;
                break;
            case "alive" :
                break;
            case "update" :
                console.log("license update service");
                var rawKey = key.toString('ascii').split('*');
                var keyJson = {
                    key : rawKey[0],
                    company : rawKey[1],
                    purchased_num : rawKey[2],
                    reserved_num : rawKey[3]
                }
                console.log(keyJson);
                dbService.addLicense(keyJson, licenses, software, socket);
                socket.destroy();
                break;
            case "display" :
                console.log("license display service");
                dbService.displayLicense(licenses, software, key, function(data){
                    socket.end(data + "\r\n");
                    socket.destroy();
                });
                break;
            default :
                console.log("unknown service");
                socket.end("unkonwn\r\n");
                socket.destroy();
                break;
        }
    });

    socket.on('close', function () {

        if (key_reserved) {
            licenseService.auto_release(theKey, theLicenses, socket);
            key_reserved = false;
        }
        this.destroy();
    });

});

//server.listen(8888, '136.159.119.220');
server.listen(8888,'127.0.0.1');