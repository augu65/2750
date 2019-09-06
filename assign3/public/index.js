let ffi=require('ffi)');



let sharedLib=ffi.library('./libcal.so',{
	'createCalendar':['ICalError',['string','Calendar**']],
	'deleteCalendar':['void',['Calendar*']],
	'printCalendar':['string',['Calendar*']],
	'printError':['string',['ICalErrorCode']],
	'writeCalendar':['ICalErrorCode',['char*','const Calendar*']],
	'validateCalendar':['ICalErrorCode',['const Calendar*']],
	'dtToJSON':['string',['DateTime']],
	'eventToJSON':['string',['const Event*']],
	'eventListToJSON':['string',['const List*']],
	'calendarToJSON':['string',['const Calendar*']],
	'JSONtoCalendar':['Calendar*',['const string']],
	'JSONtoEvent':['Event*',['const string']],
	'addEvent':['void',['Calendar*','Calendar*']],
});


// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
    // On page-load AJAX Example
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/someendpoint',   //The server endpoint we are connecting to
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            $('#blah').html("On page load, Received string '"+JSON.stringify(data)+"' from server");
            //We write the object to the console to show that the request was successful
            console.log(data); 

        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
    });

    // Event listener form replacement example, building a Single-Page-App, no redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Callback from the form");
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the 
        $.ajax({});
    });
});

