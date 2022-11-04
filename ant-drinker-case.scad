/*
 * @author  Igor Demyanov
 * @license CC-BY-NC-4.0
 * @version 0.1.0
 */

version = "23.10.2022 v0.1.0";

// Ширина платы, mm
pcb_width = 100;
// Высота платы
pcb_height = 100;
pcb_thickness = 1.6;
tollerant_size = 0.01;
pcb_mount_d = 3 + tollerant_size * 2;



module plotPCB(){
    left_edge_hole = 12.7;
    hole_h = 7.62;
    hole_w = 20.32;
    screen_d = 2;
    difference(){
    color("black") cube([pcb_width,pcb_height,pcb_thickness]);
    color("red") {
    translate([5,5,-tollerant_size]) cylinder(h=pcb_thickness+tollerant_size*2, r = pcb_mount_d/2, $fn=50);
    translate([pcb_width-5,5,-tollerant_size]) cylinder(h=pcb_thickness+tollerant_size*2, r = pcb_mount_d/2, $fn=50);
    translate([pcb_width-5,pcb_height-5,-tollerant_size]) cylinder(h=pcb_thickness+tollerant_size*2, r = pcb_mount_d/2, $fn=50);
    translate([5,pcb_height-5,-tollerant_size]) cylinder(h=pcb_thickness+tollerant_size*2, r = pcb_mount_d/2, $fn=50);
    translate([left_edge_hole,pcb_height-hole_h,-tollerant_size]) cube([hole_w,hole_h+tollerant_size,pcb_thickness+tollerant_size*2]);
        
        }
    }
    color("red"){
          translate([5,20,pcb_thickness-tollerant_size]) {
            linear_extrude(height = 0.4, convexity = 20)
                {
                    text(version, 8);
                }
            }
        }
}

module plotCase(){
}

plotPCB();
plotCase();