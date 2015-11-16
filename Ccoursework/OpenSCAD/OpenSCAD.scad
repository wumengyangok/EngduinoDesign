module bottom(){
    $fn=50;
    difference(){
        union(){
            difference(){
                minkowski(){
                    cube([85,50,5], center = false);
                    cylinder(r=5);
                }
                translate([65,-10,-1])minkowski(){
                    cube([30,30,7], center = false);
                    cylinder(r=5);
                }
            }
            translate([-5,49,0])cube([2,6,6],center = false);
            translate([10,55,3])rotate([0,0,45])cube([15*sqrt(2),15*sqrt(2),6],center = true);
            translate([37,55,3])rotate([0,0,45])cube([10*sqrt(2),10*sqrt(2),6],center = true);
            translate([66,51,3])rotate([0,0,45])cube([10+18*sqrt(2),18*sqrt(2)+4,6],center = true);
            translate([83,67,3])rotate([0,0,45])cube([20,10,6],center = true);
            cylinder(r=2, h=18, center = false);
        }
        translate([0,70,3])hole();
    }
}
module hole(){
    $fn=100;
    rotate([90,0,0])translate([20,0,-2])minkowski(){
        cube([35,1,50],center=false);
        cylinder(r=1.5,h=50,center=false);
    }
}
module top0(){
    $fn=50;
    difference(){
        minkowski(){
            cube([85,50,2], center = false);
            cylinder(r=5);
        }
        //translate([0,0,-1])cylinder(r=2, h=5,center=false);
    }
    translate([-5,49,0])cube([2,6,3],center = false);
    translate([10,55,1.5])rotate([0,0,45])cube([15*sqrt(2),15*sqrt(2),3],center = true);
    translate([37,55,1.5])rotate([0,0,45])cube([10*sqrt(2),10*sqrt(2),3],center = true);
    translate([66,51,1.5])rotate([0,0,45])cube([10+18*sqrt(2),18*sqrt(2)+4,3],center = true);
    //translate([83,67,1.5])rotate([0,0,45])cube([20,10,3],center = true);
}
module top1(){
    $fn=50;
    difference(){
        minkowski(){
            cube([85,50,2], center = false);
            cylinder(r=5);
        }
        translate([0,0,-1])cylinder(r=2, h=5,center=false);
    }
    translate([-5,49,0])cube([2,6,3],center = false);
    translate([10,55,1.5])rotate([0,0,45])cube([15*sqrt(2),15*sqrt(2),3],center = true);
    translate([37,55,1.5])rotate([0,0,45])cube([10*sqrt(2),10*sqrt(2),3],center = true);
    translate([66,51,1.5])rotate([0,0,45])cube([10+18*sqrt(2),18*sqrt(2)+4,3],center = true);
    translate([83,67,1.5])rotate([0,0,45])cube([20,10,3],center = true);
}
module top(){
    difference(){
        top1();
        translate([8,5,-0.1])scale([0.8,0.8,1.1])top0();
    }
}
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
    translate([15, 15, -5])rotate([180,90,90]){
        text("for Engduino USB", font = "Liberation Sans", size = 3);
    }
    translate([19.5, 15, -5])rotate([180,90,90]){
        text("for top case", font = "Liberation Sans", size = 3);
    }
    translate([0, 15, -5])rotate([180,90,90]){
        text("for bottom case", font = "Liberation Sans", size = 3);
    }
    
}
module _text(){
    translate([100, 15]){
        text("iStep Engduino watch", font = "Liberation Sans");
    }
    translate([19.5, -5, 2]){
        text("insert your waist belt", font = "Liberation Sans", size = 3);
    }   
}
bottom();
translate([0,0,50])top();
translate([120,90,20])rotate([90,270,-45])linker();
_text();
