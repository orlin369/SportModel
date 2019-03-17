/*

Copyright (c) [2019] [Orlin Dimitrov]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

function actuator_response(res) {
    var actuator_data;
    
    if (!res || (res.target.responseText == '[]')) {
        setTimeout(function () { on_change_actuator(); }, 5000);
        return;
    }
    
    actuator_data = JSON.parse(res.target.responseText);
    
    console.log(actuator_data.value);

    document.getElementById("actuator-display-value").innerHTML = actuator_data.value;
}

function on_change_actuator() {
    var actuator_value = document.getElementById("actuator-value").value;
    //document.getElementById("actuator-display-value").innerHTML = actuator_value;
    
    request = new XMLHttpRequest();
    
    if (request) {
        request.open('GET', '/api/v1/actuator?actuator_value=' + actuator_value, true);
        request.addEventListener('load', actuator_response);
        request.send();
    }
}

window.onload = function () {
    on_change_actuator();
}
