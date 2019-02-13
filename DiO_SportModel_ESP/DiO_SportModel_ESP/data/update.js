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

function GetState() {
    setValues("/api/v1/update/possible")
    setTimeout(function () {
        if (document.getElementById("remupd").innerHTML == "OK") {
            console.warn("ERROR = OK");
            document.getElementById("fileName").disabled = false;
            document.getElementById("updateButton").disabled = false;
            document.getElementById("remupdResult").innerHTML = "";
        }
    }, 2000);
}

window.onload = function () {
	load("style.css", "css", function () {
		load("microajax.js", "js", function () {
			GetState();
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

var blobSlice = File.prototype.slice || File.prototype.mozSlice || File.prototype.webkitSlice,
		  input = document.getElementById('fileName'),
		  running = false,
		  ua = navigator.userAgent.toLowerCase();

function summd5() {
	//input = document.getElementById("fileName");
	if (running) {
		return;
	}
	if (!input.files.length) {
		console.error('Please select a file');
		return;
	}
	var fileReader = new FileReader(),
			file = input.files[0],
			time;
	fileReader.onload = function (e) {
		running = false;
		if (file.size != e.target.result.length) {
			console.error('ERROR:Browser reported success but could not read the file until the end');
		} else {
			md5hash = SparkMD5.hashBinary(e.target.result);
			console.info('Finished loading!');
			console.info('Computed hash: ' + md5hash); // compute hash
			console.info('Total time: ' + (new Date().getTime() - time) + 'ms');
			document.getElementById('md5row').hidden = false;
			document.getElementById('clientHash').innerHTML = md5hash;
			document.getElementById('fileSize').innerHTML = file.size;
			setValues('/api/v1/setmd5?md5=' + md5hash + '&size=' + file.size);
		}
	};
	fileReader.onerror = function () {
		running = false;
		console.error('ERROR: FileReader onerror was triggered, maybe the browser aborted due to high memory usage');
	};
	running = true;
	console.info('Starting normal test (' + file.name + ')');
	time = new Date().getTime();
	fileReader.readAsBinaryString(file);
}