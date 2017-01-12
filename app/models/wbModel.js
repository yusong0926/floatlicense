/**
 * Created by syu on 8/28/16.
 */
var mongoose = require('mongoose');
var Schema = mongoose.Schema;


var WbSchema = new Schema({
    key : String,
    purchased_num : Number,
    reserved_num : Number,
    company : String
});

var wbModel = mongoose.model('wbModel', WbSchema);

module.exports = wbModel;