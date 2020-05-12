
#pragma once

static constexpr char wifisetupwebpage[] PROGMEM = R"=====(

<html><head>
    <style>

        .loader {
            display: none;
            top: 50%;
            left: 50%;
            position: absolute;
            transform: translate(-50%, -50%);
        }

        .showwhenloading{
            display: none;
            position: absolute;
            transform: translate(0%,-150%);
        }

        .loading {
            border: 2px solid #ccc;
            width: 60px;
            height: 60px;
            border-radius: 50%;
            border-top-color: #1ecd97;
            border-left-color: #1ecd97;
            animation: spin 1s infinite ease-in;
        }

        @keyframes spin {
            0% {
                transform: rotate(0deg);
            }

            100% {
                transform: rotate(360deg);
            }
        }


        .buttonstyle {
            color: #494949 !important;
            text-transform: uppercase;
            text-decoration: none;
            background: #ffffff;
            padding: 20px;
            border: 4px solid #494949 !important;
            display: inline-block;
            transition: all 0.4s ease 0s;
        }

        .buttonstyle:hover {
            color: #ffffff !important;
            background: #4563f6;
            border-color: #2324f6 !important;
            transition: all 0.4s ease 0s;
            cursor: pointer;
        }


        .form__group {
            position: relative;
            padding: 15px 0 0;
            margin-top: 10px;
            width: 100%;
        }

        .form__field {
            font-family: inherit;
            width: 100%;
            border: 0;
            border-bottom: 2px solid #9b9b9b;
            outline: 0;
            font-size: 1.3rem;
            color: #fff;
            padding: 7px 0;
            background: transparent;
            transition: border-color 0.2s;
        }

        .form__field::placeholder{
            color: transparent;
        }
        .form__field:placeholder-shown{
            font-size: 1.3rem;
            cursor: text;
            top: 20px;
        }

        .form__label {
            position: absolute;
            top: 0;
            display: block;
            transition: 0.2s;
            font-size: 1rem;
            color: #9b9b9b;
        }

        .form__field . form__label{
            position: absolute;
            top: 0;
            display: block;
            transition: 0.2s;
            font-size: 1rem;
            color: #11998e;
            font-weight:700;
        }


        .form__field:focus
        {
            padding-bottom: 6px;
            font-weight: 700;
            border-width: 3px;
            border-image: linear-gradient(to right, #11998e,#38ef7d);
            border-image-slice: 1;
        }

        .headline{
            color: white;
        }
        h2{
            color: white;
        }
        .notice{
            color:white;
            font-size: 0.8rem;
        }


        body {
            font-family: 'Poppins', sans-serif;
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
            font-size: 1.5rem;
            background-color:#222222;
        }

    </style>
</head>
<body>
<div id="maincontent">
    <h2 class="headline">ESP8266 WIFI Setup</h2>
    <p class="notice">
        Please Enter your WIFI data here... <br>
        Bitte Fuegen sie Ihre Wlan zugangsdaten hier ein!
    </p>
    <form>

        <div class="form__group field">
            <input type="input" class="form__field" placeholder="SSID" name="ssid" id="ssid" required />
            <label for="ssid" class="form__label">SSID</label>
        </div>
        <div class="form__group field">
            <input type="password" class="form__field" placeholder="Password" name="password" id="password" required />
            <label for="password" class="form__label">Password</label>
        </div>



    </form>

    <div class="button_cont" align="center">
        <a class="buttonstyle" rel="nofollow noopener" onclick="myFunction()">
            Save
        </a>
    </div>
</div>

<div class="showwhenloading">
    <p class="notice">Device is trying to connect to your Wifi...</p>
</div>

<div class="loader">

    <div class="loading">
    </div>
</div>


<script type="text/javascript">
    function spinner() {
        document.getElementsByClassName("showwhenloading")[0].style.display = "block";
        document.getElementsByClassName("loader")[0].style.display = "block";
        document.getElementById("maincontent").remove();
    }
</script>
<script>

    function myFunction() {

        console.log("button clicked....");


        var ssid = document.getElementById("ssid").value;
        var password = document.getElementById("password").value;

        console.log("value is:" + ssid);
        console.log("password is:" + password);

        var data ={ssid:ssid, password:password};
        var xhr = new XMLHttpRequest();
        var url = "/settings";


        xhr.onreadystatechange = function (ev) {
            if (this.onreadystatechange == 4 && this.status == 200)
            {
                console.log(xhr.responseText);
            }
        }


        xhr.open("POST", url, true);
        xhr.send(JSON.stringify(data));

        spinner();
    }


</script>

</body></html>

)=====";
