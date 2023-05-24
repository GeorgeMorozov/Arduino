async function subscribe() {
  let response = await fetch( `/stt`, {
        method: 'get',
        dataType: 'jsonp',
        headers: {
           'Accept': 'application/json',
           'Content-Type': 'application/json'
        }
    });

  if (response.status == 502) {
    // Статус 502 - это таймаут соединения;
    // возможен, когда соединение ожидало слишком долго
    // и сервер (или промежуточный прокси) закрыл его
    // давайте восстановим связь
    await subscribe();
  } else if (response.status != 200) {
    // Какая-то ошибка, покажем её
    showMessage(response.statusText);
    // Подключимся снова через секунду.
    await new Promise(resolve => setTimeout(resolve, 1000));
    await subscribe();
  } else {
    // Получим и покажем сообщение
    let responseData = await response.json();
    document.getElementById("switch").checked = responseData.STT =='LOW';
    // И снова вызовем subscribe() для получения следующего сообщения
	await new Promise(resolve => setTimeout(resolve, 1000));
    await subscribe();
  }
}

subscribe();


function toggleButtonSwitch(e) {
  var switchButton = document.getElementById("switch");
  
  var toggleValue = "";
  if (switchButton.checked) {
    console.log("On!");
    toggleValue = "LOW";
  } else {
    console.log("Off!");
    toggleValue = "HIGH"
  }
  fetch( `/toggle?status=${toggleValue}`)
  .then( response => {
    console.log(response);
  } )
}
/*
fetch( `/stt`, {
        method: 'get',
        dataType: 'jsonp',
        headers: {
           'Accept': 'application/json',
           'Content-Type': 'application/json'
        }
    })
  .then((response) => {
       return response.json()
    })
    .then((responseData) => {
			document.getElementById("switch").checked = responseData.STT =='LOW';
        return responseData;
    })
  .catch(function(err) {
      console.log(err);
  })
*/
