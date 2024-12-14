// globalUser.js
let user = {
  username: '',
  //password: '',
  temp_th: '',
  humi_th: '',
  lsens_th: '',
  ppm_th: ''
};

function setUser(key, value) {
  if (user.hasOwnProperty(key)) {
    user[key] = value;
  } else {
    throw new Error(`Property ${key} does not exist on user.`);
  }
}

function getUser(key) {
  if (user.hasOwnProperty(key)) {
    return user[key];
  } else {
    throw new Error(`Property ${key} does not exist on user.`);
  }
}

function getAll() {
	return user;
}


function resetUser() {
  for (let key in user) {
    user[key] = '';
  }
}

module.exports = {
  setUser,
  getUser,
  resetUser,
  getAll
};
