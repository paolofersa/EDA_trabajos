let myMap = L.map('myMap').setView([51.505, -0.09], 13)

L.tileLayer(`https://maps.wikimedia.org/osm-intl/{z}/{x}/{y}.png`, {
	maxZoom: 18,
}).addTo(myMap);

myMap.doubleClickZoom.disable()

var polygon = L.polygon([
    [51.509, -0.08],
    [51.503, -0.06],
    [51.51, -0.047]
], color='#800026').addTo(myMap);

// define rectangle geographical bounds
var bounds = [[51.509, -0.08], [51.503, -0.06]];
var colorRect = "#800026";
// create a rectangle
L.rectangle(bounds, {color: colorRect, weight: 1}).addTo(myMap);

getData();

//leyendo el archivo
async function getData(){
    const response = await fetch("../build/provisional.csv");
    const data = await response.text();
    console.log(data);

    const rows = data.split('\n');
    
}