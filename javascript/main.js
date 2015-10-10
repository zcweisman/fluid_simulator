var frames          = 0;
var force           = 5;
var source          = 100;
var displaySize     = 768;
var running         = false;
var mouseIsDown     = false;
var sources         = [];
var start           = new Date;
var frames          = 0;
var omx, omy;
var mx, my;
var res;
var fieldRes;
var canvas;

onmouseup = function() { mouseIsDown = false; }

function log(message) {
    document.getElementById("log").innerHTML = message;
}

function prepareFrame( field ) {
    var canvas = document.getElementById("canvas");
    if ((omx >= 0 && omx < displaySize && omy >= 0 && omy < displaySize) && mouseIsDown) {
        var dx = mx - omx;
        var dy = my - omy;
        var length = (Math.sqrt(dx * dx + dy * dy) + 0.5) | 0;
        if (length < 1) length = 1;
        for (var i = 0; i < length; i++) {
            var x = (((omx + dx * (i / length)) / displaySize) * field.width()) | 0
            var y = (((omy + dy * (i / length)) / displaySize) * field.height()) | 0;
            field.setVelocity(x, y, dx, dy);
            field.setDensity(x, y, 500);
        }
        omx = mx;
        omy = my;
    }
    for (var i = 0; i < sources.length; i++) {
            var x = ((sources[i][0] / displaySize) * field.width()) | 0;
            var y = ((sources[i][1] / displaySize) * field.height()) | 0;
            field.setDensity(x, y, 30);
    }
}

function stopAnimation() {
    running = false;
    clearTimeout(interval);
}

function startAnimation() {
    if (running)
        return;
    running=true;
    interval = setTimeout(updateFrame, 10);
}

function updateFrame() {
    field.update();
    //if (running)
        interval = setTimeout(updateFrame, 10);
}

window.onload=function(){
    canvas = document.getElementById("canvas");
    field = new FluidField(canvas);
    res = document.getElementById("resolution");
    field.setUICallback(prepareFrame);
    updateRes = function() {
        var r = 64;
        canvas.width = r;
        canvas.height = r;
        console.log("canvas.width: " + r);
        fieldRes = r;
    }
    updateRes();
    function getTopLeftOfElement(element) {
        var top = 0;
        var left = 0;
        do {
          top += element.offsetTop;
          left += element.offsetLeft;
        } while(element = element.offsetParent);
        return {left: left, top: top};
    }

    canvas.onmousedown = function(event) {
        var o = getTopLeftOfElement(canvas);
        omx = mx = event.clientX - o.left;
        omy = my = event.clientY - o.top;
        if (!event.altKey && event.button == 0)
            mouseIsDown = true;
        else
            sources.push([mx, my]);
        //event.preventDefault();
        return false;
    }
    canvas.onmousemove = function(event) {
        var o = getTopLeftOfElement(canvas);
        mx = event.clientX - o.left;
        my = event.clientY - o.top;
    }
    field.setDisplayFunction( display(canvas) );
    startAnimation();
}
