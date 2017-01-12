/**
 * Created by syu on 8/28/16.
 */
var mongoose = require('mongoose');
var Schema = mongoose.Schema;


var BosSchema = new Schema({
    key : String,
    purchased_num : Number,
    reserved_num : Number,
    company : String
});

var bosModel = mongoose.model('bosModel', BosSchema);

module.exports = bosModel;