const char header[] = R"=====(HTTP/1.1 200 OK
Content-type:text/html
\r\n\r\n
)=====";

const char page[] = R"=====(
<head><meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        .directional-buttons {
        width: 16rem;
        height: 16rem;
        display: grid;
        grid-gap: 0.5rem;
        grid-template-columns: 45fr 60fr 45fr;
        grid-template-rows: 45fr 60fr 45fr;
        grid-template-areas: "....  up  ....."
                            "left  ..  right"
                            ".... down .....";    
        }

        .direction-button {
          color: rgb(55, 50, 50);
          background: currentcolor;
          border: none;
          position: relative;
        }

        .direction-button:before {
          display: block;
          content: "";
          position: absolute;
          width: 4.25rem;
          height: 4.25rem;
          transform: rotate(45deg);
          background: currentcolor;
        }

        .direction-button:after {
          display: block;
          content: "";
          position: absolute;
          border: 2rem solid transparent;
          color: rgba(255, 250, 250, 0.6);
        }

        .direction-button:hover {
          color: rgb(85,55,55);
        }

        .up {
          grid-area: up;
          border-radius: 0.5rem 0.5rem 0 0;
        }
        .up:before {
          left: calc(50% - 2.125rem);
          bottom: -2.125rem;
        }
        .up:after {
          border-bottom-color: currentcolor;
          left: calc(50% - 2rem);
          top: -1rem;
        }

        .left {
          grid-area: left;
          border-radius: 0.5rem 0 0 0.5rem;
        }
        .left:before {
          right: -2.125rem;
          top: calc(50% - 2.125rem);
        }
        .left:after {
          border-right-color: currentcolor;
          top: calc(50% - 2rem);
          left: -1rem;
        }

        .right {
          grid-area: right;
          border-radius: 0 0.5rem 0.5rem 0;
        }
        .right:before {
          left: -2.125rem;
          top: calc(50% - 2.125rem);
        }
        .right:after {
          border-left-color: currentcolor;
          top: calc(50% - 2rem);
          right: -1rem;
        }

        .down {
          grid-area: down;
          border-radius: 0 0 0.5rem 0.5rem;
        }
        .down:before {
          left: calc(50% - 2.125rem);
          top: -2.125rem;
        }
        .down:after {
          border-top-color: currentcolor;
          left: calc(50% - 2rem);
          bottom: -1rem;
        }

        .us-bar {
          width: 100%;
          background-color: #e0e0e0;
          padding: 3px;
          border-radius: 3px;
          box-shadow: inset 0 1px 3px rgba(0, 0, 0, .2);
        }
        .us-bar-fill {
          display: block;
          height: 22px;
          background-color: #659cef;
          border-radius: 3px;
          transition: width 500ms ease-in-out;
        }
        .headline{
          font-size: xx-large;
          display: flex;
          align-items: center;
          justify-content: center;
          margin: 1em;
        }
        .container {
          display: flex;
          align-items: center;
          justify-content: center;
        }
        html {
          width: 100%;
          height: 100%;
          display: flex;
          align-items: center;
          justify-content: center;
          background: darkgrey;
        }
    </style>
</head>
<script>
    function gamepad(sender) {
        var theID = sender.id;
        var ajaxRequest = new XMLHttpRequest();
        ajaxRequest.open("GET", theID, true);
        ajaxRequest.onreadystatechange = function() {
          if(ajaxRequest.readyState == 4 && ajaxRequest.status == 200) {
              var ajaxResult = ajaxRequest.responseText;
          }
        }
        ajaxRequest.send();
    }

    var intervalId = window.setInterval(function(){
        var ajaxRequest = new XMLHttpRequest();
        ajaxRequest.open("GET", "pollUS", true);
        ajaxRequest.onreadystatechange = function() {
            if(ajaxRequest.readyState == 4 && ajaxRequest.status == 200) {
                var ajaxResult = ajaxRequest.responseText;
                const obj = JSON.parse(ajaxResult);
                for (var i = 0; i < 3; i++) {
                  var us = Math.min(1.0, obj["US"+(i+1)]/2.0)*100;
                  if (us < 0) us = 100;
                  document.getElementById("us" + (i+1)).setAttribute("style","width:" + us + "%");
                }
                console.log(ajaxResult);
            }
        }
        ajaxRequest.send();
    /// call your function here
    }, 200);

</script>
<body>
    <div class="headline">Hardware Praktikum SS25</div>
    <div class="container">
        <div class="directional-buttons">
            <button class="direction-button up" id='up' onclick='gamepad(this)'>
            </button>
            <button class="direction-button left" id='left' onclick='gamepad(this)'>
            </button>
            <button class="direction-button right" id='right' onclick='gamepad(this)'>
            </button>
            <button class="direction-button down" id='back' onclick='gamepad(this)'>
            </button>
        </div>
    </div>

    <div class="container" style="margin-top: 30px">
        <span style="margin-right: 10px;">US1:</span>
        <div class="us-bar">
            <span class="us-bar-fill" id='us1' style="width: 70%;"></span>
        </div>
    </div>
    <div class="container" style="margin-top: 10px">
        <span style="margin-right: 10px;">US2:</span>
        <div class="us-bar">
            <span class="us-bar-fill" id='us2' style="width: 70%;"></span>
        </div>
    </div>
    <div class="container" style="margin-top: 10px">
        <span style="margin-right: 10px;">US3:</span>
        <div class="us-bar">
            <span class="us-bar-fill" id='us3' style="width: 70%;"></span>
        </div>
    </div>
</body>
</html>)=====";
