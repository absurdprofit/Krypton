// document.getElementById("input").addEventListener("oninput", function () {
//     console.log(this.value);
//   });
window.onbeforeunload = function () {
  return "Press back again to exit.";
};
window.onunload = function () {
  return "Press back again to exit.";
};
var Module;
const setupEnv = () => {
  const canvas = document.getElementById("canvas");
  canvas.style.width = `${screen.width}px`;
  canvas.style.height = `${screen.height}px`;
  canvas.width = screen.width;
  canvas.height = screen.height;

  window.scrollTo(0, document.body.scrollHeight);

  Module = {
    canvas: (function () {
      canvas.addEventListener(
        "webglcontextlost",
        function (e) {
          alert("WebGL context lost. You will need to reload the page.");
          e.preventDefault();
        },
        false
      );

      return canvas;
    })(),
    // onRuntimeInitialized: function () {
    //   Module.main(screen.width, screen.height);
    // },
  };
};

renderWarning = () => {
  if (!document.getElementById("warning")) {
    const div = document.getElementById("container");
    const innerDiv = document.createElement("div");
    innerDiv.textContent = "This app needs a mobile device to run";
    innerDiv.setAttribute("class", "warning");
    innerDiv.setAttribute("id", "warning");
    div.appendChild(innerDiv);
  }
};
if (
  navigator.userAgent.match(/Android/i) ||
  navigator.userAgent.match(/webOS/i) ||
  navigator.userAgent.match(/iPhone/i) ||
  navigator.userAgent.match(/iPad/i) ||
  navigator.userAgent.match(/iPod/i) ||
  navigator.userAgent.match(/BlackBerry/i) ||
  navigator.userAgent.match(/Windows Phone/i)
) {
  setupEnv();
} else {
  if (Module) {
    Module.clean;
  }
  canvas.width = `${0}`;
  canvas.height = `${0}`;
  canvas.style.width = `${0}px`;
  canvas.style.height = `${0}px`;
  renderWarning();
}
const resize = () => {
  if (
    navigator.userAgent.match(/Android/i) ||
    navigator.userAgent.match(/webOS/i) ||
    navigator.userAgent.match(/iPhone/i) ||
    navigator.userAgent.match(/iPad/i) ||
    navigator.userAgent.match(/iPod/i) ||
    navigator.userAgent.match(/BlackBerry/i) ||
    navigator.userAgent.match(/Windows Phone/i)
  ) {
    const warning = document.getElementById("warning");
    if (warning) {
      warning.textContent = "Reload the page to restart the application.";
      location.reload();
    }
  } else {
    if (Module) {
      Module.clean();
    }
    canvas.width = `${0}`;
    canvas.height = `${0}`;
    canvas.style.width = `${0}px`;
    canvas.style.height = `${0}px`;

    renderWarning();
  }
};

document.body.addEventListener("fullscreenchange", function () {
  if (document.fullscreenElement) {
    console.log("Fullscreen");
  } else {
    console.log("Exit Fullscreen");
  }
  const canvas = document.getElementById("canvas");
  canvas.style.width = `100%`;
  canvas.style.height = `100%`;
  canvas.width = document.body.style.width;
  canvas.height = document.body.style.height;
});