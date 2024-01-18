


difference() {
//Grundkoerper
cube(size = [90,50,5], center = true);

//Platine
color("red") 
translate(v = [14.5, 0.25, -1.1]) {
cube(size = [62,41,3.5], center = true);
    }
    
    translate(v = [41.1, 0.25, 0.3]) {
cube(size = [8,28,5], center = true);
}

}

/*
color("black") 
translate(v = [14.5, 0, -0.9]) {
cube(size = [61,40,3], center = true);
}  
*/


