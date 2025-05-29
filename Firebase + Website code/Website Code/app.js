// Complete Project Details at: https://RandomNerdTutorials.com/

// Database Paths
var gsrpath = 'GSR/value';
var temppath = 'temp/value';
var xpath = 'AccXX/value';
var ypath = 'AccYY/value';
var zpath = 'AccZZ/value';


// Get a database reference 
const gsrvalue = database.ref(gsrpath);
const tempvalue = database.ref(temppath);
const xvalue = database.ref(xpath);
const yvalue = database.ref(ypath);
const zvalue = database.ref(zpath);


// Variables to save database current values
var gsrreading;
var tempreading;
var xreading;
var yreading;
var zreading;


// Attach an asynchronous callback to read the data
gsrvalue.on('value', (snapshot) => {
    gsrreading = snapshot.val();
    console.log(gsrreading);
    document.getElementById("gsr-int").innerHTML = gsrreading;
  }, (errorObject) => {
    console.log('The read failed: ' + errorObject.name);
  });

tempvalue.on('value', (snapshot) => {
    tempreading = snapshot.val();
  console.log(tempreading);
  document.getElementById("temp-float").innerHTML = tempreading;
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});

xvalue.on('value', (snapshot) => {
    xreading = snapshot.val();
  console.log(xreading);
  document.getElementById("x-float").innerHTML = xreading;
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});

yvalue.on('value', (snapshot) => {
    yreading = snapshot.val();
  console.log(yreading);
  document.getElementById("y-float").innerHTML = yreading;
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});

zvalue.on('value', (snapshot) => {
    zreading = snapshot.val();
  console.log(zreading);
  document.getElementById("z-float").innerHTML = zreading;
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});



let alertShown = false;

tempvalue.on('value', (snapshot) => {
  tempreading = snapshot.val();
  console.log(tempreading);
  document.getElementById("temp-float").innerHTML = tempreading;

  // Check if tempreading is greater than or equal to 38
  if (tempreading >= 38 && !alertShown) {
      // Change body background color to red
      document.body.style.backgroundColor = '#C90D0D';

      // Display alert message with content "danger"
      alert("Warning!! Seizure Case Detected. See written instructions in the AI chatbot on the left");

      // Set alertShown to true to prevent the alert from showing again
      alertShown = true;
      
  } else if (tempreading < 38) {
      // Reset body background color to default (if needed)
      document.body.style.backgroundColor = ''; // Set to default color

      // Set alertShown to false to allow the alert to show again
      alertShown = false;
  }
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});
