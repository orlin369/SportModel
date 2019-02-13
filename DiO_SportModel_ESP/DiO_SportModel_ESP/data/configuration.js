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

function securityStr(security) {
    if      (security == 7) {
        return 'Open';
    }
    else if (security == 5) {
        return 'WEP';
    }
    else if (security == 2) {
        return 'WPA';
    }
    else if (security == 4) {
        return 'WPA2';
    }
    else if (security == 8) {
        return 'WPA/WPA2';
    }
}

function wifiScan(res) {
    var array;

    if (!res || (res.target.responseText == '[]')) {
        setTimeout(function () { getNetwork(); }, 5000);
        return;
    }
    array = JSON.parse(res.target.responseText);
    array.sort(function (a, b) { return a.rssi - b.rssi });
    array.reverse();
    document.getElementById("numNets").innerHTML = array.length;
    var table = document.getElementById("networks");
    table.innerHTML = "";
    for (var i = 0; i < array.length; i++) {
        var row = document.createElement("tr");
        row.innerHTML = "<td><a href='javascript:selssid(\"" + array[i].ssid + "\")'>" + array[i].ssid + "</td><td>" + array[i].channel + "</td><td>" + securityStr(array[i].secure) + "</td><td>" + array[i].rssi + "</td>";
        table.appendChild(row);
    }
}

function getNetwork() {
    request = new XMLHttpRequest();
    if (request) {
        request.open("GET", "/api/v1/scanNetworks", true);
        request.addEventListener("load", wifiScan)
        request.send();
    }
}

function getState() {
    setValues("/api/v1/connectionState");
}
function selssid(value) {
    document.getElementById("ssid").value = value;
}


        function restartESP() {
            setValues("/admin/restart");
        }

window.onload = function () {
    load("style.css", "css", function () {
        load("microajax.js", "js", function () {
            setValues("/admin/values");
            setTimeout(getState, 3000);
            setTimeout(getNetwork, 1000);
        });
    });
}

function load(e, t, n) {
    if ("js" == t) {
        var a = document.createElement("script");
        a.src = e,
        a.type = "text/javascript",
        a.async = !1,
        a.onload = function () { n() },
        document.getElementsByTagName("head")[0].appendChild(a)
    } else if ("css" == t) {
        var a = document.createElement("link");
        a.href = e,
        a.rel = "stylesheet",
        a.type = "text/css",
        a.async = !1,
        a.onload = function () { n() },
        document.getElementsByTagName("head")[0].appendChild(a)
    }
}

