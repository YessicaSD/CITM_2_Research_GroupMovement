
I am  <a href="https://www.linkedin.com/in/yessica-servin-dominguez-663175165/" >Yessica Servin Dominguez</a>, student of the <a href="https://www.citm.upc.edu/ing/estudis/graus-videojocs/">Bachelor’s Degree in Video Games by UPC at CITM</a>. 
This content is generated for the second year’s subject Project 2, under supervision of lecturer <a href="https://es.linkedin.com/in/ricardpillosu">Ricard Pillosu</a>. 
<p>Especial thanks to Sandra Alvarez, she also did the same research so check out her project <a href="https://sandruski.github.io/RTS-Group-Movement/" >here</a><p>

<h1>Introduction</h1>
<p>The objetive of this research is to implement group pathfinding. 
To reach this we'll analysis differents group movement implented in RTS games, we'll see different forms to implemantate it.
This feature's something that you can do in many ways, it depends on your game characteristics.</p>
<i>Disclaimer: Although this implementation is based on different RTS games it could also be applied in different kind of games. 
 AND please check out reference links I used to develop this project you could find more information</i>
 <iframe width="560" height="315" src="https://www.youtube.com/embed/eswjTK_6-A4" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
 <i> Espected Result</i>
 
 <h1>Pathfinding</h1>
 To implement a group movement the first step is to be able to move one unit, this one needs to find the best way to point A to B, the best way to do this is by using the <a href="https://www.redblobgames.com/pathfinding/a-star/introduction.html">A* algorithm</a> , it's generally used in most of the games cause it takes lowe computation power, and if you need a lot of units to be searching for the goal that is an important thing to consider. 
 If you are a beginner and your game doesn't need that many units, so you can also see Breadth First Search algorithm or Dijkstra’s Algorithm. These methods are more easy to understand and also to implement and a good point to start. 
 
 <h1>Goal Tile</h1>
<p>If we try units to move to the same Tile they would get overlapping, or if we got collision system only one unit would get there and the other was will still try to move<p>
  <img src="https://media.indiedb.com/images/articles/1/254/253140/auto/8mfFxGS.gif"/>
 
 To solve this one idea is to transfer the state of "found goal" through units, so if a unit tries to get in a tile were there is one already but both of them got the same the destination and one found it the other one will too, and stop moving.
  <img src="https://i.imgur.com/9zImMiX.gif"/>
 
 This implementation is called "transitive bumping" and it's a good idea but cause some problems like:
 
 - Units placed in line instead of a group
 <img src=" https://i.imgur.com/9zImMiX.gif"/>

So what we are going to do is determine an offset to the goal so every entity got its own goal. This offset is calculated with the start position of every unit. We find the middle center between entities and translate this point to the destination and place entities respecting the original distance between this one and the middle point.

 <p>If you don't see it yet here's a gif to illustrate this</p>
  
  <img src="https://github.com/YessicaSD/CITM_2_Research_GroupMovement/blob/master/docs/Media/GroupMovement.gif?raw=true" alt="GifExplenation" width="560" align="center"/>
<br/>
   <i>The blue point is the middle point and the red one is the destination, and squares are entities</i>
   
This is something that games like Command & Conquer: Red Alert 2 and The Maestros seems to be doing.  
<h1>Command & Conquer:Red Alert 2</h1>
<p>
 <ul style="list-style-type:disc;">
  <li>In this game units set they objetive before they start moving.</li>
  <li>The units try to mantein the inital positions. The rest of them select as their goal nearby that tile but following the same direcction as the orginal tile and parallel to this.</li>
  <li>Also some of the units got the same goel so when one unit reach the goel the other ones stop when they are next to that one.</li>
</ul>
 </p>
 
<iframe width="560" height="315" src="https://www.youtube.com/embed/ap3odnteoIs?start=57" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

<h1>The Maestros</h1>
<p> As I said before in this game units mantein the inital formaton.
  In this game, you could see that on the feet of every entity selected there is a circle this is used as the area that an entity take up and it is considered to determinate the position of the entities. So when an entity is going to a tile to know that this one has reached it we watch if that point was inside this area.<p>
<iframe width="560" height="315" src="https://www.youtube.com/embed/cLW7W1IzpJw?start=62" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
 
   
   <p>The transitive bumping is something that  works really well when the group is small and at the start of the movement they are near each other, also it's a really simple implemantation<p>
 
 <p>The problem with this is that as they started so far away from the group they end up separated, instead to join the group. To solve this problem we are going to limit the offset of the entities to the goal.
In my case I made a Lider in case a unit don't have a path cause it's goal was invalid or was out of this ratio of offset units can follow.
 <br>
  <img src="https://github.com/YessicaSD/CITM_2_Research_GroupMovement/blob/master/docs/Media/NoFollowingLeader.gif?raw=true" width="400"/> 
 <img src="https://github.com/YessicaSD/CITM_2_Research_GroupMovement/blob/master/docs/Media/FollowLider.gif?raw=true" width="400"/> 
 <br>
 <i>before and after</i>
 <br>
<p>
 
 
   <h1>Collition system</h1>
   For preventing entities to collide to each other, we are going to not let them collide in the first place. 
   To do this before they start moving we are going to consider all units, their state, and their move. We are going to identify which kind of collision the unity is going to have so we can prevent it.
   
   
<h3>First kind of collition </h3>
<p>
<i>An unit try to enter in a tile where there is a unit not moving.</i>
To solve this problem you should consider de direction of the unit that is moving.
To move the one that lies still in a tile that doesn't bother the unit that is moving. Or if you want to make the effect that one is pushing the other, you should do the opposite make. 
<img align="left" https://github.com/YessicaSD/CITM_2_Research_GroupMovement/blob/master/docs/Media/Captura.PNG"/>
<p>
<img src="https://github.com/YessicaSD/CITM_2_Research_GroupMovement/blob/master/docs/Media/MovingUnits.gif?raw=true" width="400" align="left">
  <img src="https://github.com/YessicaSD/CITM_2_Research_GroupMovement/blob/master/docs/Media/pushingUnits.gif?raw=true" width="400" >
 <br>
 
 <iframe width="560" height="315" src="https://www.youtube.com/embed/jA2epda-RkM?start=78" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen align="center"></iframe>
 <br>
 <i>This is done in the "Supreme Commander 2" in witch units move away from the path of other </i>
 
 <h3>Second kind of collition </h3>
<i>Two units want to get into the same tile, in this one there is no unit.</i>
<br>
<p>To solve this possible collition one unit should wait the other one to move, to this tile and get out.
Also consider if this tile is the goal of one of them, in which case the first unit will pass would be the one that it's not.
You probably won't notice this implementation depending on the units speed.<p>
 <img align="left" src="https://github.com/YessicaSD/CITM_2_Research_GroupMovement/blob/master/docs/Media/Captura2.PNG?raw=true"/>

<h3> Third kind of collition</h3>
<i>Two agents reach the cell of each other</i>
Happens if the agents are walking towards each other (in opposite directions).
<img align="left" src="https://github.com/YessicaSD/CITM_2_Research_GroupMovement/blob/master/docs/Media/Captura3.PNG?raw=true"/>
 
<h2>Reference information</h2>
<ul style="list-style-type:disc;">
  <li> <a href="https://www.gamasutra.com/blogs/AndrewErridge/20180522/318413/Group_Pathfinding__Movement_in_RTS_Style_Games.php" >https://www.gamasutra.com/blogs/AndrewErridge/20180522/318413/Group_Pathfinding__Movement_in_RTS_Style_Games.php</a></li>
 
   <li> <a href="https://www.gamasutra.com/view/feature/3314/coordinated_unit_movement.php?print=1" >https://www.gamasutra.com/view/feature/3314/coordinated_unit_movement.php?print=1</a></li>
 
 <li> <a href="https://www.gamasutra.com/view/feature/3313/coordinated_unit_movement.php?print=1">https://www.gamasutra.com/view/feature/3313/coordinated_unit_movement.php?print=1</a></li> 
 
 <li> <a href="https://sandruski.github.io/RTS-Group-Movement/">https://sandruski.github.io/RTS-Group-Movement/</a></li> 
</ul>



