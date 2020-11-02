function display(){
const database = firebase.database();
var userId = firebase.auth().currentUser.uid;
console.log(userId);

const usersRef = database.ref('/users/'+ userId +'/items');




usersRef.on('value', snapshot => {
    user_info = snapshot.val()
    
    var rfid = [];
    var item_name = [];
    var status = [];

    for (const key of Object.keys(user_info)) {
        rfid.push(key);
        console.log(key);
            if(user_info[key]['status']=='1'){
                status.push("Inside the Bag");
            }
            else{
                status.push("Not in the Bag");
            }
            
            //Object info = user_info[key]
            item_name.push(user_info[key]['name']);
            
          
      }
console.log(rfid);
console.log(item_name);
console.log(status);
var table="<tr><th>RF-ID</th><th>ITEM NAME</th><th>STATUS</th></tr>";
for (var i=0; i<item_name.length; i++) {
    table+= "<tr><td> " + rfid[i] + "</td>";
    table +="<td>" + item_name[i] + "</td>";
    table +="<td>" + status[i] + "</td></tr>";
  }
  console.log(table)
  document.getElementById("demo").innerHTML = table;
});


  
}



