
I am  <a href="https://www.linkedin.com/in/yessica-servin-dominguez-663175165/" >Yessica Servin Dominguez</a>, student of the <a href="https://www.citm.upc.edu/ing/estudis/graus-videojocs/">Bachelor’s Degree in Video Games by UPC at CITM</a>. 
This content is generated for the second year’s subject Project 2, under supervision of lecturer <a href="https://es.linkedin.com/in/ricardpillosu">Ricard Pillosu</a>.

<h1>Introduction</h1>
<p>The objetive of this research is to implement group pathfinding. To reach this we'll analisy differents group movement implented in RTS games, we'll get the best caracteristics of each one or most of them and create the best implementation posible.</p>
<i>Although this implamenteation is based on diferents RTS games it could also be applaid in differens kind of games.</i>

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
 <p>
<iframe width="560" height="315" src="https://www.youtube.com/embed/cLW7W1IzpJw?start=62" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
  

<h2>Reference information</h2>
<ul style="list-style-type:disc;">
  <li> <a href="https://www.gamasutra.com/blogs/AndrewErridge/20180522/318413/Group_Pathfinding__Movement_in_RTS_Style_Games.php" >https://www.gamasutra.com/blogs/AndrewErridge/20180522/318413/Group_Pathfinding__Movement_in_RTS_Style_Games.php</a></li>
 
   <li> <a href="https://www.gamasutra.com/view/feature/3314/coordinated_unit_movement.php?print=1" >https://www.gamasutra.com/view/feature/3314/coordinated_unit_movement.php?print=1</a></li>
 
 <li> <a href="https://www.gamasutra.com/view/feature/3313/coordinated_unit_movement.php?print=1">https://www.gamasutra.com/view/feature/3313/coordinated_unit_movement.php?print=1</a></li> 
 
</ul>
