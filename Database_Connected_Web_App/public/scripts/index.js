// Database Paths
var currentSamplePath = "currentSample";
var currentSampleNamePath = "currentSample/name";
var currentSampleNumberPath = "currentSample/number";
var currentTestNamePath = "currentTest/name";
var currentTestNumberPath = "currentTest/number";
var namesOfSamplesPath = "namesOfSamples";
var numberOfSamplesPath = 'numberOfSamples';
var testButtonPath = 'testButton';
var testsPath = 'tests';

// Get a database reference
const databaseTests = database.ref(testsPath);
const databaseNumberOfSamples = database.ref(numberOfSamplesPath);
const databaseNamesOfSamples = database.ref(namesOfSamplesPath);
const databaseCurrentSample = database.ref(currentSamplePath);
const databaseCurrentTestNumber = database.ref(currentTestNumberPath);

// Database references
var databaseButton = firebase.database().ref().child(testButtonPath);
var currentSampleNumberButton = firebase.database().ref().child(currentSampleNumberPath);
var currentSampleNameButton = firebase.database().ref().child(currentSampleNamePath);
var currentTestNameButton = firebase.database().ref().child(currentTestNamePath);
var currentTestNumberButton = firebase.database().ref().child(currentTestNumberPath);
var namesOfSamplesButton = firebase.database().ref().child(namesOfSamplesPath);
var numberOfSamplesButton = firebase.database().ref().child(numberOfSamplesPath);
var testsButton = firebase.database().ref().child(testsPath);
const tableBody = document.getElementById("tbody");

// Variables to save database current values
var testsReading;
var humidityReading;
var temperatureReading;
var timeOfFlightReading;
var namesOfSamplesReading;
var numberOfSamplesReading;
var currentSampleReading;
var currentTestNumberReading;
var value = 0;

// Getting initial value of testButton
firebase.database().ref().child("testButton").get().then((snapshot) => {
  if (snapshot.exists()) {
    if (snapshot.val() == 0) {
      value = 1;
    } else {
      value = 0;
    }
    console.log(snapshot.val());
  } else {
    console.log("No data available");
  }
}).catch((error) => {
  console.error(error);
});


// ------------------- Asynchronous Data Callbacks --------------------- //

databaseCurrentTestNumber.on('value', (snapshot) => {
  currentTestNumberReading = snapshot.val();
  console.log(currentTestNumberReading);
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});

databaseCurrentSample.on('value', (snapshot) => {
  currentSampleReading = snapshot.val();
  console.log(currentSampleReading);
  document.getElementById("current-sample").innerHTML = currentSampleReading["name"];
  document.getElementById("current-sample-title").innerHTML = currentSampleReading["name"];
  createTable();
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});

databaseNumberOfSamples.on('value', (snapshot) => {
  numberOfSamplesReading = Number(snapshot.val());
  console.log(numberOfSamplesReading);
  const element = document.getElementById("wood-samples");
  element.innerHTML = '';
  for (let i = 0; i < numberOfSamplesReading; i++) {
    var index = i + 1;
    var indexString = index.toString();
    var idString = "reading-sampleName" + indexString;
    var buttonString = "sampleButton" + indexString;
    var innerHTMLString = "<button type='button' class='button-on' id='" + buttonString + "' onclick= 'sampleChange(" + indexString + ")'>" + indexString + "</button> <span id='"+ idString + "'></span>";
    const newElem = Object.assign(document.createElement("p"), 
    { id: indexString, style: "text-align:left;text-indent: 25px", innerHTML: innerHTMLString});
    element.appendChild(newElem);
  }
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});

databaseNamesOfSamples.on('value', (snapshot) => {
  namesOfSamplesReading = snapshot.val();
  console.log(namesOfSamplesReading);
  var index = 1;
  var indexString;
  var idString;
  for (var key in namesOfSamplesReading) {
    indexString = index.toString();
    idString = "reading-sampleName" + indexString;
    document.getElementById(idString).innerHTML = namesOfSamplesReading[key];
    index = index + 1;
  }
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});

databaseTests.on('value', (snapshot) => {
  testsReading = snapshot.val();
  createTable();
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});


// ------------------- Button Functions --------------------- //

function newTest() {
  if (document.getElementById("input-test-name").value == '') {
    document.getElementById("no-test-name-warning").style.display = 'block';
  } else {
    document.getElementById("no-test-name-warning").style.display = 'none';
    currentTestNameButton.set(document.getElementById("input-test-name").value);
    databaseButton.set(value);
    if (value == 0) {
      value = 1;
    } else {
      value = 0;
    }
    document.getElementById("input-test-name").value = '';
  }
}

function newSample() {
  if (document.getElementById("input-sample-name").value == '') {
    document.getElementById("no-sample-name-warning").style.display = 'block';
  } else {
    document.getElementById("no-sample-name-warning").style.display = 'none';
    numberOfSamplesReading = numberOfSamplesReading + 1;
    numberOfSamplesButton.set(numberOfSamplesReading);
    namesOfSamplesReading.push(document.getElementById("input-sample-name").value);
    namesOfSamplesButton.set(namesOfSamplesReading);
    currentTestNumberReading.push(0);
    currentTestNumberButton.set(currentTestNumberReading);
    document.getElementById("input-sample-name").value = '';
  }
}

function sampleChange(num) {
  currentSampleNumberButton.set(num);
  currentSampleNameButton.set(namesOfSamplesReading[num]);
}

function createTable() {
  tableBody.innerHTML = '';
  if (currentTestNumberReading[currentSampleReading['number']] == 0) {
    var content = '';
    content += '<tr>';
    content += '<td>' + 'none' + '</td>';
    content += '<td>' + 'none' + '</td>';
    content += '<td>' + 'none' + '</td>';
    content += '<td>' + 'none' + '</td>';
    content += '</tr>';
    tableBody.innerHTML += content;
    document.getElementById("current-test-name").innerHTML = 'No';
    document.getElementById("reading-timeOfFlight").innerHTML = 'none';
    document.getElementById("reading-temperature").innerHTML = 'none';
    document.getElementById("reading-humidity").innerHTML = 'none';
  } else {
    var currentSampleNumberString = currentSampleReading["number"].toString();
    var currentSampleTestsPath = "tests/" + currentSampleNumberString;
    var name;
    var timeOfFlight;
    var temperature;
    var humidity;
    const databaseCurrentSampleTests = database.ref(currentSampleTestsPath);
    databaseCurrentSampleTests.orderByKey().on('child_added', function(snapshot) {
      if (snapshot.exists()) {
        var jsonData = snapshot.toJSON();
        console.log(jsonData);
        name = jsonData.name.toString();
        timeOfFlight = jsonData.timeOfFlight.toString();
        temperature = jsonData.temperature.toString();
        humidity = jsonData.humidity.toString();
        var content = '';
        content += '<tr>';
        content += '<td>' + name + '</td>';
        if (timeOfFlight == 0) {
          content += '<td>' + "err" + '</td>';
        } else {
          content += '<td>' + timeOfFlight + '</td>';
        }
        if (temperature == 0) {
          content += '<td>' + "err" + '</td>';
        } else {
          content += '<td>' + temperature + '</td>';
        }
        if (humidity == 0) {
          content += '<td>' + "err" + '</td>';
        } else {
          content += '<td>' + humidity + '</td>';
        }
        content += '</tr>';
        tableBody.innerHTML += content;
      }
    });
    document.getElementById("current-test-name").innerHTML = name;
    if (timeOfFlight == 0) {
      document.getElementById("reading-timeOfFlight").innerHTML = 'Error';
    } else {
      document.getElementById("reading-timeOfFlight").innerHTML = timeOfFlight + ' (us)';
    }
    if (temperature == 0) {
      document.getElementById("reading-temperature").innerHTML = 'Error';
    } else {
      document.getElementById("reading-temperature").innerHTML = temperature + ' (F)';
    }
    if (humidity == 0) {
      document.getElementById("reading-humidity").innerHTML = 'Error';
    } else {
      document.getElementById("reading-humidity").innerHTML = humidity + '%';
    }
  }
}

function deleteSample() {
  // Unfinished
}

function deleteTest() {
  // Unfinished
}
