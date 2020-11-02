setTimeout(function get_userinfo(){
const rfid = document.getElementById('userId');
localStorage.setItem("rfid",rfid);
console.log(rfid.value);
const itemname = document.getElementById('firstName');
console.log(itemname.value);
localStorage.setItem("itemname",itemname);
const database = firebase.database();
const usersRef = database.ref('/users');
userId = firebase.auth().currentUser.uid;
console.log(userId);
//var userId ="nj01SAuD7yN7GAbhW0uMTSqZfDk1";

},3000);
addBtn.addEventListener('click', e => {
    const rfid = document.getElementById('userId');
    const itemname = document.getElementById('firstName');
  e.preventDefault();
  firebase.database().ref('users/' + userId + '/items/' + rfid.value +'/').set({
    name: itemname.value,
    status:0
  });
  
});

removeBtn.addEventListener('click', e => {
    const rfid = document.getElementById('userId');
    const itemname = document.getElementById('firstName');
    e.preventDefault();
    firebase.database().ref('/').child('users/' + userId + '/items/' + rfid.value ).remove();
});

// setTimeout( function usersRef.on('value', snapshot => {
//     console.log('Child added !');
// }); 3000);

// usersRef.on('child_added', snapshot => {
//   console.log(snapshot.val());
// });