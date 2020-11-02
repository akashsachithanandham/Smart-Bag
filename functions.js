const userId = document.getElementById('userId');
const firstName = document.getElementById('firstName');
const lastName = document.getElementById('lastName');
const age = document.getElementById('age');
const addBtn = document.getElementById('addBtn');
const updateBtn = document.getElementById('updateBtn');
const removeBtn = document.getElementById('removeBtn'); 

const database = firebase.database();
const usersRef = database.ref('/users');


// const usersRef = database.ref('/users');
// const normalUsersRef = usersRef.child('normal_users');
// const superUsersRef = usersRef.child('super_users');

addBtn.addEventListener('click', e => {
  e.preventDefault();
  firebase.database().ref('users/' + userId.value).set({
    first_name: firstName.value,
    last_name: lastName.value,
    age: age.value
  });
  // usersRef.child(userId.value).set({
  //   first_name: firstName.value,
  //   last_name: lastName.value,
  //   age: age.value
  // });
});
removeBtn.addEventListener('click', e => {
  e.preventDefault();
  usersRef.child(userId.value).remove();
});
usersRef.on('value', snapshot => {
    console.log('Child added !');
});

usersRef.on('child_added', snapshot => {
  console.log(snapshot.val());
});