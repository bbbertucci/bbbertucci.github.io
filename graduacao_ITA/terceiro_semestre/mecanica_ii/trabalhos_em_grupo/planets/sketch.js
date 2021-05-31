function setup() {
	pixelDensity(1);
	frameRate(60);
	createCanvas(1000,1000);
	
	orbit = createGraphics(600,600);
	trajectory = createGraphics(600,600);
	
	sliderM = createSlider(1,15,10);
	sliderM.position(650,100);
	
	sliderR = createSlider(20,120,50);
	sliderR.position(650,150);
	
	sliderVr = createSlider(-30,30,0);
	sliderVr.position(650,200);
	
	sliderVtheta = createSlider(-20,20,5);
	sliderVtheta.position(650,250);
	
	sliderK = createSlider(0,30,2);
	sliderK.position(650,300);
	
	buttonF = createButton("Trocar força");
	buttonF.position(650,50);
	buttonF.mousePressed(switch_force);
	
	
	buttonR = createButton("Reset");
	buttonR.position(850,50);
	buttonR.mousePressed(reset_orbit);
	
	buttonM = createButton("Mostrar corpo central");
	buttonM.position(1000,50);
	buttonM.mousePressed(show_central);
	
	buttonD = createButton("Defaults");
	buttonD.position(850,100);
	buttonD.mousePressed(default_params);
	
	
	buttonp1 = createButton("preset1");
	buttonp1.position(650,500);
	buttonp1.mousePressed(preset1);
	
	buttonp2 = createButton("preset2");
	buttonp2.position(750,500);
	buttonp2.mousePressed(preset2);
	
	buttonp3 = createButton("preset3");
	buttonp3.position(850,500);
	buttonp3.mousePressed(preset3);
	
	buttonp4 = createButton("preset4");
	buttonp4.position(950,500);
	buttonp4.mousePressed(preset4);
	
	buttonp5 = createButton("preset5");
	buttonp5.position(650,550);
	buttonp5.mousePressed(preset5);
	
	buttonp6 = createButton("preset6");
	buttonp6.position(750,550);
	buttonp6.mousePressed(preset6);
	
	buttonp7 = createButton("preset7");
	buttonp7.position(850,550);
	buttonp7.mousePressed(preset7);
	
	buttonp8 = createButton("preset8");
	buttonp8.position(950,550);
	buttonp8.mousePressed(preset8);
	
	buttonp9 = createButton("preset9");
	buttonp9.position(650,600);
	buttonp9.mousePressed(preset9);

	buttonp10 = createButton("preset10");
	buttonp10.position(750,600);
	buttonp10.mousePressed(preset10);
	
	orbit.translate(300,300);
}

let PI = 3.14159265;

// parametros iniciais em coordenadas polares
let r = 50;
let v_r = 0;
let a_r = 0;
let theta = 0;
let v_theta = .0005;
let a_theta = 0;
let m = 10;
let showM = true;

// método de Euler
let iterations = 1000;
let timestep = 60/(iterations);


// parametros de simulação
// let v_theta = -L/(2500*mass);
let n=2;
let trajectory_x = [];
let trajectory_y = [];
let K=.2;

function draw() {
	background(255);
	draw_orbit();
	draw_trajectory();
	
	strokeWeight(1);
	text("F = -K/r^" + n, buttonF.x, buttonF.y - 10);
	text("m = " + sliderM.value(), sliderM.x, sliderM.y);
	text("r_0 = " + sliderR.value(), sliderR.x, sliderR.y);
	text("Vr_0 = " + sliderVr.value() + "e-03", sliderVr.x, sliderVr.y);
	text("Vtheta_0 = " + sliderVtheta.value() + "e-04", sliderVtheta.x, sliderVtheta.y);
	text("K = " + sliderK.value() + "e-01", sliderK.x, sliderK.y);
	text("Momento angular (L) = " + round(100*m*r*r*v_theta)/100, sliderK.x, sliderK.y + 50);
}


function update_position() {
	for(let i = 0; i < iterations; i++) {
	a_r = r*v_theta*v_theta - K/(m*pow(r,n));
	a_theta = -2*v_r*v_theta/r;	
	
	v_r += a_r*timestep;
	r += v_r*timestep;
	v_theta += a_theta*timestep;
	theta += v_theta*timestep;
	}
}

function draw_trajectory() {
	let x = r*cos(theta);
	let y = r*sin(theta);

	// desenha a trajetória do corpo
	trajectory_x.unshift(x);
	trajectory_y.unshift(y);
	
	if(trajectory_x.length > 900) {
		trajectory_x.pop();
		trajectory_y.pop();
	}
	trajectory.clear();
	trajectory.beginShape();
	trajectory.strokeWeight(2);
	trajectory.stroke(233,127,2);
	trajectory.noFill();
	for(let i=0; i<trajectory_x.length; i++) {
		trajectory.vertex(trajectory_x[i]+300,trajectory_y[i]+300);
	}
	trajectory.endShape();
	image(trajectory,0,0);
}

function draw_orbit() {
	// desenha a orbita
	orbit.background(73,10,61);
	orbit.noStroke();
	orbit.fill(189,21,80);
	if(showM) {
		orbit.ellipse(0,0,20,20);
	}
	
	update_position();
	
	let x = r*cos(theta);
	let y = r*sin(theta);
	orbit.ellipse(x,y,m,m);
	image(orbit,0,0);
}

function reset_orbit() {
	// reinicia a orbita quando os parâmetros são modificados
	theta = 0;
	m = sliderM.value();
	r = sliderR.value();
	v_r = sliderVr.value()/1000;
	v_theta = sliderVtheta.value()/10000;
	K = sliderK.value()/10;
	
	trajectory_x = [];
	trajectory_y = [];
}

function switch_force() {
	if(n == 2) {
		n = 3;
	} else if(n == 3) {
		n = 2;
	}
	reset_orbit();
}

function show_central() {
	showM = !showM;
}

function default_params() {
	m = 10;
	sliderM.value(10);
	sliderR.value(50);
	sliderVtheta.value(5);
	sliderVr.value(0);
	sliderK.value(2);
	theta = 0;
	n=2;
	reset_orbit();
}

function preset1() {
	default_params();
	reset_orbit();
}

function preset2() {
	default_params()
	sliderR.value(43);
	reset_orbit();
}

function preset3() {
	default_params()
	sliderR.value(53);
	reset_orbit();
}

function preset4() {
	default_params()
	sliderR.value(54);
	reset_orbit();
}

function preset5() {
	default_params()
	sliderVtheta.value(2);
	sliderK.value(25);
	n=3;
	reset_orbit();
}

function preset6() {
	default_params();
	sliderM.value(8);
	sliderR.value(51);
	sliderVtheta.value(2);
	sliderK.value(22);
	n=3;
	reset_orbit();
}

function preset7() {
	default_params();
	sliderM.value(8);
	sliderR.value(52);
	sliderVtheta.value(2);
	sliderK.value(23);
	n=3;
	reset_orbit();
}

function preset8() {
	default_params();
	sliderM.value(8);
	sliderR.value(90);
	sliderVtheta.value(1);
	sliderVr.value(-30);
	sliderK.value(23);
	n=3
	reset_orbit();
}

function preset9() {
	default_params();
	sliderM.value(8);
	sliderR.value(77);
	sliderVtheta.value(1);
	sliderVr.value(-30);
	sliderK.value(23);
	n=3
	reset_orbit();
}

function preset10() {
	default_params();
	sliderM.value(8);
	sliderR.value(84);
	sliderVtheta.value(1);
	sliderVr.value(-20);
	sliderK.value(30);
	n=3
	reset_orbit();
}
