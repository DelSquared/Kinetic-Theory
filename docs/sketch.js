var m = 5;
var w = 300;
var h = 300;
var n = 100;
var speed = 10;
var damping = 1;
var csq = 0;
var tSlider;
var t;


function Particle (m) {
    this.m=m,
    this.r = Math.floor(Math.random() * 255),
    this.g = Math.floor(Math.random() * 255),
    this.b = Math.floor(Math.random() * 255),
    this.x=Math.floor(Math.random()*3)+w/2,
    this.y=Math.floor(Math.random()*3)+h/2,
    this.vx=3*Math.random(),
    this.vy=-3*Math.random(),
    this.collision=(p) => {
      return (this.x-p.x)*(this.x-p.x) + (this.y-p.y)*(this.y-p.y)}
    this.Step=()=>{
        this.x+=speed*this.vx/(frameRate()+1);
        this.y+=speed*this.vy/(frameRate()+1);
    }
};

var p = new Array(n);
for (i=0;i<n;i++){
  p[i]=new Particle(m);
}

function setup() {
  createCanvas(w, h+25);  //canvas setup
  background(0);
  frameRate(100);
  fill("white");
  stroke("black");
  strokeWeight(2);
  rect(0,h,w,25);
  for (i=0;i<n;i++){
    strokeWeight(p[i].m);
    stroke(p[i].r,p[i].g,p[i].b);
    point(p[i].x,p[i].y);
  }
  tSlider = createSlider(0,1000,100);
  tSlider.position(w/2+20,h+8);
  t=tSlider.value()
}

function draw() {
  background(0);
  fill("white");
  stroke("black");
  strokeWeight(2);
  rect(0,h,w,25);
  t=tSlider.value()
  csq=0;
  for (i=0;i<n;i++){
    p[i].Step();
    csq+=(p[i].vx*p[i].vx+p[i].vy*p[i].vy)/n;
    if (csq!=3*t/m){
      damping=(1 - 1/frameRate()) + Math.sqrt(3*t/m)/(frameRate()*csq);
    }
    if (p[i].x <= m/2 || p[i].x >= w-m/2) {
      p[i].vx=-damping*p[i].vx;
      p[i].Step();
      p[i].Step();

    }
    if (p[i].y <= m/2 || p[i].y >= h-m/2) {
      p[i].vy=-damping*p[i].vy;
      p[i].Step();
      p[i].Step();

    }
    strokeWeight(p[i].m);
    stroke(p[i].r,p[i].g,p[i].b);
    point(p[i].x,p[i].y);
  }
  fill("black");
  strokeWeight(0);
  text("<c^2> = " + Math.round(100*csq)/100 + ", T = " + t,10,h+15);
  for (i=0;i<n;i++){
    for (j=i;j<n;j++){
      if (Math.floor(2*p[i].x/w)==Math.floor(2*p[j].x/w) || Math.floor(2*p[i].y/h)==Math.floor(2*p[j].y/h)){
        if (p[i].collision(p[j])<25){
          var sx = (p[i].vx*p[i].m/p[j].m);
          var sy = (p[i].vy*p[i].m/p[j].m);
          p[i].vx = (p[j].vx*p[j].m/p[i].m);
          p[i].vy = (p[j].vy*p[j].m/p[i].m);
          p[j].vx = sx;
          p[j].vy = sy;
        }
      }

    }

  }

}

//d(Tg-T)/dt = -(Tg-T)
//D = (1 - 1/FR) + sq(3T/m)/FR*c
