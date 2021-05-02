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
var p1x, p1y, p2x, p2y, col;
//leyendo el archivo
async function getData(){
    const response = await fetch('../build/provisional.txt')
    const data = await response.text();

    const rows = data.split('\n');
    rows.forEach(i=>{
        const row = i.split('\t');
        p1x = row[0];
        p1y = row[1];
        p2x = row[2];
        p2y = row[3];
        col = row[4];
        console.log(p1x,p1y);
    });

    console.log(rows);
}