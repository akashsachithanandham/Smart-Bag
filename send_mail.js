const accountSid = 'ACc00852adb5e766c5615aab982d6ff9fe';
const authToken = '6206aca2121eaacda584ff4a4cafe0b9';
const client = require('twilio')(accountSid, authToken);
const user_mobile = '9384445383';
client.messages
      .create({from: '+15017122661', body: '', to: user_mobile})
      .then(message => console.log(message.sid));