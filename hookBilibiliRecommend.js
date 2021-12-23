// ==UserScript==
// @name         hookBilibiliRecommend
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  disallow bilibili to auto play recommended video
// @author       wwc7654321
// @match        https://www.bilibili.com/video/*
// @icon         https://www.google.com/s2/favicons?domain=bilibili.com
// @license      MIT
// @grant        none
// ==/UserScript==
console.log(localStorage['hkeri']);
(function() {
    'use strict';
    window.hker={};
    window.hker.mousestate=0;
    window.addEventListener('mousedown',function(){window.hker.mousestate=1;});
    window.addEventListener('mouseup',function(){setTimeout(function(){window.hker.mousestate=0;},3000);});
    function PauseMe(){
        console.log('pause');
        if($('.video-state-pause').length==0){
            $('.bilibili-player-video-btn-start').click();
        }
    }
    window.hker.hookJump=function(){
        setTimeout(
            function(){
                console.log('hooking');
                if(Date.now()-localStorage['hkeri']<15000){
                    PauseMe();
                    localStorage['hkeri']=0;
                }
                window.hker.wwps=window.history.pushState;
                window.hker.bvid0=window.bvid;
                window.history.pushState=function(a,b,c){
                    var r=c.match('/video/(\\w*)[\\?/].*');
                    if(!window.hker.mousestate && r && r[1]!=window.hker.bvid0){
                        console.log('not allow',c,r,window.hker.bvid0);
                        window.hker.location=window.location.href;
                        localStorage['hkeri']=Date.now();
                        setTimeout(function(){window.location=window.hker.location;},1000);
                        return false;
                    }else{
                        console.log('allow',window.hker.mousestate,c,r[1],window.hker.bvid0);
                        window.history.pushState=window.hker.wwps;
                        localStorage['hkeri']=0;
                        var t=window.hker.wwps(a,b,c);
                        window.hker.hookJump();
                        return t;
                    }
                };
            } , 5000);
        return true;
    };
    window.hker.hookJump();
})();
