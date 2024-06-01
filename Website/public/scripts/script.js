//Button
const ON = document.querySelector('[data-button="ON"]');
const OFF = document.querySelector('[data-button="OFF"]');

ON.onclick = () => {
    const data = {
        LED : (perempuan.value == "true")
    }
    firebase.database().ref().update(data);
}

OFF.onclick = () => {
    const data = {
        LED : (OFF.value == "true")
    }
    firebase.database().ref().update(data);
}

//Slider
const slideValue = document.querySelector("span");
const inputSlider = document.querySelector("input");
inputSlider.oninput = (()=>{
    let value = parseInt(inputSlider.value);
    let scaledValue = Math.round((value / 100) * 255);
    slideValue.textContent = value + "%";

    firebase.database().ref().update({
        'speed-motor' : scaledValue
    });
});