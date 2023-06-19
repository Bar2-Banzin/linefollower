# <img align=center  height=50px src="https://media3.giphy.com/media/v1.Y2lkPTc5MGI3NjExZTU2ZTZjZGE1MmQ1ZWNiMjM1Yzc0NTEyODQ4NjVlODRiMTkyODcyMyZlcD12MV9pbnRlcm5hbF9naWZzX2dpZklkJmN0PXM/xcw3xOLyHmfrQleoi7/giphy.gif"> Line Follower

<div align="center">

   <img width=450px  src="https://github.com/Bar2-Banzin/linefollower/assets/71986226/a02b9db9-7244-4801-aca3-8438c15cfea7" alt="logo">

  ### Our god save Azza. 🏎️

</div>

<p align="center"> 
    <br> 
</p>

## <img align= center width=50px height=50px src="https://user-images.githubusercontent.com/71986226/178468913-84c3151c-e8d1-4420-a9e9-b2434e9c2f75.gif"> Table of Contents

- <a href ="#about"> 📙 Overview</a>
- <a href ="#Components"> :money_with_wings: Components & cost</a>
- <a href ="#parts"> ⚙️  Parts</a>
    - <a href ="#app"> 📱 App</a>
    - <a href ="#car"> 🚗 Car</a>
- <a href ="#Circuit">💡 Circuit</a>
- <a href ="#Integration">🤖 Integration</a>
- <a href ="#Contributors"> ✨ Contributors</a>
- <a href ="#License"> 🔒 License</a> 
<hr style="background-color: #4b4c60"></hr>

<a id = "about"></a>

## <img align="center"  width =50px  height =50px src="https://c.tenor.com/BU6EP0XgvzEAAAAj/cat-cute.gif"> Overview
<ul>
<li>Azza is line follower robot and it's target to finish track as soon as possible</li>
<li>There is App help Azza to find straight roads to run with the heighth speed. App communicate with Azza by bluetooth </li>
<li>Azza can accomplish this by utilising a combination of closed-loop controllers such as PID and a variety of conditions</li>


<li>🎉 This project ranked 1st place among 7 teams. </li>
<li>This project built using. </li>
<ul>
<li><a href="https://docs.arduino.cc/">Arduino</a></li>
<li><a href="https://docs.flutter.dev/">Flutter</a></li>
<li><a href="https://devdocs.io/cpp/">C++</a></li>
</ul>
<li>
 The Project have 2 parts
</li>
<ul>
<li> <a href ="#app"> 📱 App</a></li>
 <li>  <a href ="#car"> 🚗 Car</a></li>
</ul>
</ul>
<hr style="background-color: #4b4c60"></hr>
<a id = "Components"></a>

##  <img align="center"  width =70px  height =70px src="https://media1.giphy.com/media/HWhlNhHNN7zGwMfGV4/giphy.gif?cid=ecf05e47351ain7blea4l6t9oeffyqj013pkodl298lred3w&rid=giphy.gif&ct=s"> Components & cost

| Name| Number | Market Cost per Part                                                                                                         |
| --------------  | ------ | ---------------------------------------------------------------------------------------------------------------------------- |
| Arduino Uno    | 1      | 356 EGP                                               |
| 1298 motor driver | 1      | 60 EGP               |
| DC Motor | 2      | 35 EGP
| Car chassis | 1 | 65 EGP
| Train sensor | 1  | 130 EGP
| Wheel | 2 | 30 EGP
| Caster Wheel | 1 | 35 EGP|

### Total Budget = 776 EGP

> **Note**: All prices are recorded in 2023

<hr style="background-color: #4b4c60"></hr>

<a id = "parts"></a>

## <img align="center"  width =70px  height =70px src="https://media4.giphy.com/media/7ktwhqlnFaDAJ8AzIw/giphy.gif?cid=ecf05e47p68znkbblq8sysehj7ahivaaceyfjly28ch0y4m7&rid=giphy.gif&ct=s"> Parts 

<a id = "app"></a>

### <img align="center"  width =60px src="https://media1.giphy.com/media/SQTSmiJl0uKoZNdv1T/giphy.gif?cid=ecf05e478njktzbodwy6urk6p15ealjalm4augxqvtm6gj0h&ep=v1_stickers_search&rid=giphy.gif&ct=s"> APP


<ul>
<li>Apps find straight road to help Azza when drive with the heighth speed</li>
<li>App should be fixed while running </li>
<li>App built using flutter and use Use Dart FFI to connect to image processing code </li>
<li>Image processing code is built using C++  </li>
<li>The paper read & yellow help us to know front and back od the car. Front of the car is red. back of the car is yellow.</li>
<li>Path of App </li>
<br>
<details> <summary>Click to expand...</summary>


<table>
<thead>
<th>Screen</th>
<th>Description</th>
</thead>
<tr>
<td><img height="400px" src="https://github.com/Bar2-Banzin/linefollower/assets/71986226/078fd3e3-7e23-49f8-a307-15c4ee8138af"></td>
<td><ul><li>Start Screen</li>
<li>Press start button to use app </li>
</ul></td>
</tr>
<tr>
<td><img height="400px" src="https://github.com/Bar2-Banzin/linefollower/assets/71986226/4306b20e-2698-4e22-b5e2-ab6f4424e597"></td>
<td><ul><li>Connect to the bluetooth </li>
<li>Most of time it will HC-05 </li>
</ul></td>
</tr>
<tr>
<td><img height="400px" src="https://github.com/Bar2-Banzin/linefollower/assets/71986226/33830037-a74e-4399-b260-ef3d547ad0b9"></td>
<td><ul><li>Take picture to the track when it is empty  </li>
<li>You can zoom in or zoom out</li>
<li>Press on camera when you want to take picture</li>
</ul></td>
</tr>
<tr>
<td><img height="400px" src="https://github.com/Bar2-Banzin/linefollower/assets/71986226/4d96041f-13ae-457f-af31-028ec4a99b1a"></td>
<td><ul><li>You can zoom in or zoom out</li>
<li>When press on camera start streaming and detect car on straight road or not </li>
<li>Output:</li>
<ul>
<li>0 => not in straight road</li>
<li>1 => in straight road</li>
<li>5 => can't find car</li>
</ul>
</ul></td>
</tr>
</table>

</details>


> **Note**: Mobile should be fixed while running program 

<li>Result of image processing</li>
<br>
<table>
<thead>
<th>Input</th>
<th>Output</th>
</thead>
<tr>
<td width="50%"><img src="https://github.com/Bar2-Banzin/linefollower/assets/71986226/68bf318b-9b0c-4d00-9fc5-50baabf919bd"></td>
<td><img src="https://github.com/Bar2-Banzin/linefollower/assets/71986226/382865b4-479b-4288-8b8e-7f366063a5aa"></td>
</tr>
</table>
</ul>

<hr style="background-color: #4b4c60"></hr>
<a id = "car"></a>

### <img align="center"  width =70px src="https://media0.giphy.com/media/7hAr7m8m7k3uumlu4G/giphy.gif?cid=ecf05e47z16ab3t2pi15z2vh1bt01uqsai1f4uszvbzvn56t&ep=v1_stickers_search&rid=giphy.gif&ct=s"> Car
<ul>
<li>Azza receive output from app if found 1 drive with the fastest speed if 0 drive with original speed</li>
<li> Azza is able to tackle the line following problem using an array of sensors </li>
<li>Azza can accomplish this by utilizing a combination of closed-loop controllers such as PID and a variety of conditions</li>
</ul>
<div align ="center">
<video  width="350px" src="https://github.com/Bar2-Banzin/linefollower/assets/71986226/85fb124f-ba64-4f36-a43a-103a99c853f9
">
</div>

<hr style="background-color: #4b4c60"></hr>

<a id = "Integration"></a>

## <img align="center"  width =80px  height =80px src="https://media1.giphy.com/media/lr7lYUc00nr3hWsoVa/giphy.gif?cid=ecf05e47f99e9tbsstbe3u9mzphxjrp27jxozshi64cok467&rid=giphy.gif&ct=s"> Integration 

<div align ="center">
<video width="350px" src="https://github.com/Bar2-Banzin/linefollower/assets/71986226/2eaf5c64-732a-4eb2-8150-f153404ad527
">
</div>

<hr style="background-color: #4b4c60"></hr>

<a id ="Contributors"></a>

## <img  align="center" width= 70px height =55px src="https://media0.giphy.com/media/Xy702eMOiGGPzk4Zkd/giphy.gif?cid=ecf05e475vmf48k83bvzye3w2m2xl03iyem3tkuw2krpkb7k&rid=giphy.gif&ct=s"> Contributors 
<table align="center">
  <thead >
         <th style="text-align: center;"> Project Manager</th>
        
  </thead>
  <tr>
   <td align="center"><a href="https://github.com/Mohabz-911"><img src="https://avatars.githubusercontent.com/u/68201932?v=4" width="150px;" alt=""/><br /><sub><b>Mohab Zaghloul</b></sub></a><br /></td>
  </tr>
</table>
<br>
<table align="center">
  <tr>
   <td align="center"><a href="https://github.com/MahmoudAbdelhamidAli"><img src="https://avatars.githubusercontent.com/u/87570209?v=4" width="150px;" alt=""/><br /><sub><b>Mahmoud Abdlhamid</b></sub></a><br /></td>
   <td align="center"><a href="https://github.com/nouralmulhem"><img src="https://avatars.githubusercontent.com/u/76218033?v=4" width="150px;" alt=""/><br /><sub><b>Nour Ziad Almulhem</b></sub></a><br /></td>
      <td align="center"><a href="https://github.com/BasmaElhoseny01"><img src="https://avatars.githubusercontent.com/u/72309546?v=4" width="150px;" alt=""/><br /><sub><b>Basma Elhoseny</b></sub></a><br /></td>
      <td align="center"><a href="https://github.com/AhmedHosny2024"><img src="https://avatars.githubusercontent.com/u/76389601?v=4" width="150px;" alt=""/><br /><sub><b>Ahmed Hosny</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/ahmedmadbouly186"><img src="https://avatars.githubusercontent.com/u/66012617?v=4" width="150px;" alt=""/><br /><sub><b>Ahmed Madbouly </b></sub></a><br /></td>
    </tr>
    <tr>
      <td align="center"><a href="https://github.com/emanshahda" ><img src="https://avatars.githubusercontent.com/u/89708797?v=4" width="150px;" alt=""/><br /><sub><b>Eman Shahda</b></sub></a><br />
    </td>
        <td align="center"><a href="https://github.com/Fathi79"><img src="https://avatars.githubusercontent.com/u/96377553?v=4" width="150px;" alt=""/><br /><sub><b>Abd Elrhman Fathi</b></sub></a><br /></td>
      <td align="center"><a href="https://github.com/zeinabmoawad" ><img src="https://avatars.githubusercontent.com/u/92188433?v=4" width="150px;" alt=""/><br /><sub><b>Zeinab Moawad</b></sub></a><br />
    </td>
      <td align="center"><a href="https://github.com/doaa281" ><img src="https://avatars.githubusercontent.com/u/65799105?v=4" width="150px;" alt=""/><br /><sub><b>Doaa ElSherif</b></sub></a><br />
    </td>
       <td align="center"><a href="https://github.com/EslamAsHhraf"><img src="https://avatars.githubusercontent.com/u/71986226?v=4" width="150px;" alt=""/><br /><sub><b>Eslam Ashraf</b></sub></a><br /></td>
  
  </tr>
  
  </table>


<hr style="background-color: #4b4c60"></hr>

<a id ="License"></a>

## 🔒 License

> **Note**: This software is licensed under MIT License, See [License](https://github.com/Bar2-Banzin/linefollower/blob/arduino/LICENSE) for more information ©Lightning McQueen.
