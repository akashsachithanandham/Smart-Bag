setTimeout(function get_user(){
    const database = firebase.database();
var userId = firebase.auth().currentUser.uid;
console.log(userId);
localStorage.setItem("username",userId);


console.log(localStorage.getItem("username"));
const usersRef = database.ref('/users/'+ userId + '/location/recent');

usersRef.on('value', snapshot => {
    user_info = snapshot.val()
    console.log(user_info);
    console.log(user_info['lat']);
localStorage.setItem("longitude",user_info['long']);
localStorage.setItem("latitude",user_info['lat']);
console.log(typeof(localStorage.getItem("latitude")));
});},4000);


//window.onload = get_user();
var map;

function initMap() {
    
    
    var latitude = parseFloat(localStorage.getItem("latitude"));// YOUR LATITUDE VALUE
    var longitude = parseFloat(localStorage.getItem("longitude")); // YOUR LONGITUDE VALUE
    console.log(latitude);
    var myLatLng = {lat: latitude, lng: longitude};

    map = new google.maps.Map(document.getElementById('map'), {
      center: myLatLng,
      zoom: 14
    });

    var marker = new google.maps.Marker({
      position: myLatLng,
      map: map,
      //title: 'Hello World'

      // setting latitude & longitude as title of the marker
      // title is shown when you hover over the marker
      title: latitude + ', ' + longitude 
    });


}
        