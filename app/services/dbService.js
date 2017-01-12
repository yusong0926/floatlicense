/**
 * Created by syu on 9/1/16.
 */
var BosModel = require('../models/bosModel');
var WbModel = require('../models/wbModel');


var loadData = function(licenses, software){
    var LicenseModel = BosModel;
    switch (software) {
        case "bos" :
            LicenseModel = BosModel;
            break;
        case "wb" :
            LicenseModel = WbModel;
            break;
    }
    LicenseModel.find({}, function (err, docs) {
        console.log(docs.length);
        docs.forEach(function(doc){
            licenses[doc.key] = {
                purchased_num : doc.purchased_num,
                reserved_num : doc.reserved_num,
                company : doc.company
            }
        });
        console.log(licenses);
    });
};


var addLicense = function(newDoc, licenses, software){
    var LicenseModel = BosModel;
    switch (software) {
        case "bos" :
            LicenseModel = BosModel;
            break;
        case "wb" :
            LicenseModel = WbModel;
            break;
    }
    LicenseModel.findOne({key : newDoc.key}, function(err, doc){
        if(doc) {
            console.log("doc exist");
            doc.purchased_num = newDoc.purchased_num;
            doc.save();
            if(licenses[newDoc.key]) {
                licenses[newDoc.key].purchased_num = newDoc.purchased_num;
            } else {
                licenses[newDoc.key] = {
                    purchased_num : newDoc.purchased_num,
                    reserved_num : newDoc.reserved_num,
                    company : newDoc.company
                }
            }

        }else {
            console.log("doc not exist");
            console.log(newDoc);
            var doc = new LicenseModel({key : newDoc.key, reserved_num : newDoc.reserved_num, purchased_num : newDoc.purchased_num,
                                        company : newDoc.company
            });
            doc.save();

            licenses[newDoc.key] = {
                purchased_num : newDoc.purchased_num,
                reserved_num : newDoc.reserved_num,
                company : newDoc.company
            }
            console.log(licenses);
        }
    });
};


var displayLicense = function(licenses, software, license_key, callback) {
    var str = "";
    if (license_key === "all") {
        console.log(licenses);
        for (var key in licenses) {
            str += "key : " + key + " " + "company : " + licenses[key].company + " " + "purchased_num : " + licenses[key].purchased_num
                + " " + "reserved_num : " + licenses[key].reserved_num + "$";
        }
    } else {
        var key = license_key;
        if (key in licenses) {
            str += "key : " + key + " " + "company : " + licenses[key].company + " " + "purchased_num : " + licenses[key].purchased_num
                + " " + "reserved_num : " + licenses[key].reserved_num + "$";
        } else {
            str = "unkonwn key";
        }
    }
    console.log(str);
    callback(str);

};


module.exports = {
    loadData: loadData,
    addLicense: addLicense,
    displayLicense:displayLicense
};

