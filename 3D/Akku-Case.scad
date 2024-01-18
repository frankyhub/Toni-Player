/*
*******************************************
Objekt Info: Akku 18650 Box einfach/zweifach

*******************************************
Version: 30.04.2022 khf
*******************************************
*/

//***************   Auswahl   *************

        //Auswahl 1x Case oder 2x Case
        part="case2"; // case1, case2

//***************   Libraries  ************

//keine

//*****************************************

//***************  Parameter   *************
$fn=60;
//*****************************************

//**************   Programm  **************


module case1(){
//Case 1x
 union(){
difference()
translate(v = [0, 21, 0])
difference(){
cube(size = [70,23,14], center = true);
translate(v = [0, 0, 1])
cube(size = [66.1,19,15], center = true);
}


//+Pol
translate(v = [-39.5, 21, 2])
difference(){
sphere(d = 15);
rotate([0,0,90])
translate(v = [-10, -6, -10])
cube(size = [20,25,20]);
    }

// /*
//-Pol
 translate(v = [39.5, 21, 2])
 union(){
    difference(){
sphere(d = 15);
rotate([0,0,-90])
translate(v = [-10, -6, -10])
cube(size = [20,25,20]);
 }
}

}
}



module case2(){
//Case Nr 1
 union(){
difference()
translate(v = [0, 21, 0])
difference(){
cube(size = [70,23,14], center = true);
translate(v = [0, 0, 1])
cube(size = [66.1,19,15], center = true);
}


//+Pol
translate(v = [-39.5, 21, 2])
difference(){
sphere(d = 15);
rotate([0,0,90])
translate(v = [-10, -6, -10])
cube(size = [20,25,20]);
    }

// /*
//-Pol
 translate(v = [39.5, 21, 2])
 union(){
    difference(){
sphere(d = 15);
rotate([0,0,-90])
translate(v = [-10, -6, -10])
cube(size = [20,25,20]);
 }
}

}

//Case 2x

 translate(v = [0, 21, 0])
 union(){
difference()
translate(v = [0, 21, 0])
difference(){
cube(size = [70,23,14], center = true);
translate(v = [0, 0, 1])
cube(size = [66.1,19,15], center = true);
}


//+Pol
translate(v = [-39.5, 21, 2])
difference(){
sphere(d = 15);
rotate([0,0,90])
translate(v = [-10, -6, -10])
cube(size = [20,25,20]);
    }

//-Pol
 translate(v = [39.5, 21, 2])
 union(){
    difference(){
sphere(d = 15);
rotate([0,0,-90])
translate(v = [-10, -6, -10])
cube(size = [20,25,20]);
 }
}
}
}

//Part:

  if(part=="case1")
    case1();
  else if(part=="case2")
    case2();