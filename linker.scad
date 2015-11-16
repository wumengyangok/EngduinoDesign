module linker(){
    difference(){
        $fn=100;
        minkowski(){
            cube([21,10,15],center=false);
            cylinder(r=2,h=5,center=false);
        }
        translate([1,0,-1])cube([5,10,100]);
        translate([14,0,-1])cube([4,10,100]);
        translate([19,0,-1])cube([2,10,100]);
    }
}