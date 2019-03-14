
I am  <a href="https://www.linkedin.com/in/yessica-servin-dominguez-663175165/" >Yessica Servin Dominguez</a>, student of the <a href="https://www.citm.upc.edu/ing/estudis/graus-videojocs/">Bachelor’s Degree in Video Games by UPC at CITM</a>. 
This content is generated for the second year’s subject Project 2, under supervision of lecturer <a href="https://es.linkedin.com/in/ricardpillosu">Ricard Pillosu</a>.

<h1>Introduction</h1>
<p>The objetive of this research is to implement group pathfinding. 
To reach this we'll analisy differents group movement implented in RTS games, we'll see differents forms to implemantate it.
This fiature it's something that you can do in many ways, it depens in your game caracteristics.</p>
<i>Disclaimer: Although this implamenteation is based on diferents RTS games it could also be applaid in differens kind of games. 
 AND please check out reference links I used to develop this project you could find more information</i>

<h1>Command & Conquer:Red Alert 2</h1>
<p>
 <ul style="list-style-type:disc;">
  <li>In this game units set they objetive before they start moving.</li>
  <li> Althought the player click on one tile only one unity will reach place, the rest of them select as their goal nearby that tile but following the same direcction as the orginal tile and parallel to this.</li>
  <li>Also some of the units got the same goel so when one unit reach the goel the other ones stop when they are next to that unit.</li>
</ul>
 </p>
 
<iframe width="560" height="315" src="https://www.youtube.com/embed/ap3odnteoIs?start=57" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

<h1>The Maestros</h1>
<p>
 One thing to point out on this game is that the solution they gave to not have multiple units trying to reach the same destinetion is to mantein the locations they have between each other. To do this easist way is to find the middle center between entities and translate this point to the destination and place entities respect this one, respecting the original distance.
 The problem with this is that sometimes some units don't get a path, case the it's goal was invalid.
 In this game you could see that on the feets of every entity selected there is a circle this is used as the area that a entity ocuped and it is considered to determinate the position of the enitites. So when a entity is going to a tile to know that this one has reached it we watch if that point was inside this area.
 <p>
<iframe width="560" height="315" src="https://www.youtube.com/embed/cLW7W1IzpJw?start=62" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
  <p>If you don't see it yet heres a gif to illustrate this</p>
  
  <img src="https://github.com/YessicaSD/CITM_2_Research_GroupMovement/blob/master/docs/Media/GroupMovement.gif?raw=true" alt="GifExplenation" width="560" align="center"/>
<br/>
   <i>The blue point is the middle point and the red one is the destination, and squares are entities</i>
   
   <p>Also this works really well when the group of unit is small and at the start of the movement they are near echa other, also it's a really simple implemantation<p>
   
   <h1>Collition system</h1>
   For preventing entities to collide to each other, we are going to not let them collide in the first place. 
   To do this before they stat moving we are going to consider all units, their state and their move. We are going to identify witch kind of collition the unity is going to have so we can prevent it.
   
   
<h3>First kind of collition </h3>
<p>
<i>An unit try to enter in a tile where there is a unit not moving.</i>
To solve this problem you should consider de direction of the unit that is moving.
To move the one that lies stile in a tile that doesn't bother the unit that is moving. Or if you want to make the effect that one is pushing the other, you should do the oposite make. 
<p>
<img src="https://github.com/YessicaSD/CITM_2_Research_GroupMovement/blob/master/docs/Media/MovingUnits.gif?raw=true" width="400" align="left">
  <img src="https://github.com/YessicaSD/CITM_2_Research_GroupMovement/blob/master/docs/Media/pushingUnits.gif?raw=true" width="400" >
 <br>
 
 <h3>Second kind of collition </h3>
<i>Two units want to get into the same tile, in this one there is no unit.</i>
<p>To solve this possible collition one unit should wait the other one to move, to this tile and get out.
Also consider if this tile is the goal of one off them, in witch case the first unit will pass would be the one that it's not.<p>
 
 
2. Two agents reach the cell of each other (TowardsCell): occurs if the agents are walking towards each other (in opposite directions).

 
<h2>Reference information</h2>
<ul style="list-style-type:disc;">
  <li> <a href="https://www.gamasutra.com/blogs/AndrewErridge/20180522/318413/Group_Pathfinding__Movement_in_RTS_Style_Games.php" >https://www.gamasutra.com/blogs/AndrewErridge/20180522/318413/Group_Pathfinding__Movement_in_RTS_Style_Games.php</a></li>
 
   <li> <a href="https://www.gamasutra.com/view/feature/3314/coordinated_unit_movement.php?print=1" >https://www.gamasutra.com/view/feature/3314/coordinated_unit_movement.php?print=1</a></li>
 
 <li> <a href="https://www.gamasutra.com/view/feature/3313/coordinated_unit_movement.php?print=1">https://www.gamasutra.com/view/feature/3313/coordinated_unit_movement.php?print=1</a></li> 
 
</ul>


