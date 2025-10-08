const effectIcons = ['block', 'print', 'arrow_upward', 'arrow_downward', 'arrow_back',
'arrow_forward', 'animation', 'grid_on', 'blur_on', 'grain', 'view_week',
'shuffle', 'waterfall_chart', 'keyboard_tab', 'deselect', 'table_chart',
'height', 'align_vertical_bottom', 'open_in_full', 'keyboard_return',
'fullscreen_exit', 'undo', 'north_east', 'north_west', 'south_east', 'south_west',
'vertical_align_top', 'vertical_align_bottom'];

const effectNames = ['No Effect', 'Print', 'Scroll Up', 'Scroll Down', 'Scroll Left', 'Scroll Right',
'Slice', 'Mesh', 'Fade', 'Dissolve', 'Blinds', 'Random', 'Wipe', 'Wipe Cursor',
'Scan Horiz', 'Scan HorizX', 'Scan Vert', 'Scan VertX', 'Opening', 'Opening Cursor',
'Closing', 'Closing Cursor', 'Scroll Up Left', 'Scroll Up Right', 'Scroll Down Left',
'Scroll Down Right', 'Grow Up', 'Grow Down'];

var BRIGHT_MIN = 1, BRIGHT_MAX = 15, SPEED_MIN = 1, SPEED_MAX = 10;

let statusData = {
    brightness: 5,
    speed: 5,
    effect: 0,
    message: '',
    ip: '',
    curtime: '--:--:--',
    ssid: '...'
};

function isLoggedIn() {
    return localStorage.getItem('parola_token') !== null;
}

function showLogin() {
    document.getElementById('login-modal').style.display = 'flex';
    document.getElementById('errbox').textContent = '';
}

function hideLogin() {
    document.getElementById('login-modal').style.display = 'none';
}

function checkAuth() {
    if (!isLoggedIn()) {
        showLogin();
        return false;
    }
    return true;
}

function showToast(msg) {
    var t = document.getElementById('toast');
    t.innerText = msg;
    t.style.display = 'block';
    setTimeout(function() {
        t.style.display = 'none';
    }, 1900);
}

function pillFillColor(type, value, min, max) {
    var pct = (value - min) / (max - min);
    if (type === 'bright') {
        if (pct < 0.34) return '#13e872';
        if (pct < 0.67) return '#fca638';
        return '#fc3460';
    }
    if (type === 'speed') {
        if (pct < 0.34) return '#13e872';
        if (pct < 0.67) return '#fca638';
        return '#fc3460';
    }
    return '#1ec2ed';
}

function setPillSlider(id, value, min, max) {
    var fill = document.getElementById(id + '-fill'),
        span = document.getElementById(id + 'val'),
        pct = (value - min) / (max - min);
    fill.style.width = Math.round(pct * 100) + '%';
    fill.style.background = pillFillColor(id, value, min, max);
    span.innerText = value;
}

function pillSliderHandler(id, min, max) {
    var wrap = document.getElementById(id + '-slider'),
        fill = document.getElementById(id + '-fill'),
        bar = fill.parentNode,
        down = false;

    function updateFromPos(x) {
        var rect = bar.getBoundingClientRect();
        var rel = (x - rect.left) / rect.width;
        rel = Math.max(0, Math.min(rel, 1));
        var value = Math.round(min + rel * (max - min));
        setPillSlider(id, value, min, max);
        if (checkAuth()) {
            fetch('/set' + id.charAt(0).toUpperCase() + id.slice(1), {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                    'Authorization': 'Bearer ' + localStorage.getItem('parola_token')
                },
                body: 'value=' + value
            });
            showToast((id == 'bright' ? 'Brightness: ' : 'Speed: ') + value);
        }
    }

    wrap.addEventListener('mousedown', function(e) {
        down = true;
        updateFromPos(e.clientX);
    });
    wrap.addEventListener('touchstart', function(e) {
        down = true;
        updateFromPos(e.touches[0].clientX);
    });
    document.addEventListener('mousemove', function(e) {
        if (down) updateFromPos(e.clientX);
    });
    document.addEventListener('touchmove', function(e) {
        if (down) updateFromPos(e.touches[0].clientX);
    }, {passive: false});
    document.addEventListener('mouseup', function() {
        down = false;
    });
    document.addEventListener('touchend', function() {
        down = false;
    });
    wrap.addEventListener('click', function(e) {
        updateFromPos(e.clientX);
    });
}

function fetchStatus() {
    if (!checkAuth()) {
        showLogin();
        return;
    }
    fetch('/status', {
        headers: {
            'Authorization': 'Bearer ' + localStorage.getItem('parola_token')
        }
    })
    .then(function(r) {
        return r.json();
    })
    .then(function(d) {
        statusData = d;
        setPillSlider('bright', d.brightness || 5, BRIGHT_MIN, BRIGHT_MAX);
        setPillSlider('speed', d.speed || 5, SPEED_MIN, SPEED_MAX);
        document.getElementById('msg').value = d.message;
        Array.prototype.forEach.call(document.querySelectorAll('.fx-cell'), function(el, ix) {
            el.classList.toggle('fx-selected', ix === d.effect);
        });
        document.getElementById('ip').innerText = d.ip;
        document.getElementById('curtime').innerText = d.curtime;
        document.getElementById('ssid').innerText = (d.ssid || 'WiFi');
        document.getElementById('uptime').innerText = d.uptime;
        updateWifiStatus(d);
    })
    .catch(function() {
        showLogin();
    });
}

function setFxGallery() {
    var fx = document.getElementById('fxgallery');
    fx.innerHTML = '';
    for (var i = 0; i < effectNames.length; ++i) {
        var cd = document.createElement('div');
        cd.className = 'fx-cell';
        cd.id = 'fx' + i;
        var ic = document.createElement('i');
        ic.className = 'material-icons fx-icon';
        ic.innerText = effectIcons[i];
        var lb = document.createElement('span');
        lb.className = 'fx-label';
        lb.innerText = effectNames[i];
        cd.appendChild(ic);
        cd.appendChild(lb);
        (function(i) {
            cd.addEventListener('click', function() {
                if (!checkAuth()) return;
                fetch('/setEffect', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/x-www-form-urlencoded',
                        'Authorization': 'Bearer ' + localStorage.getItem('parola_token')
                    },
                    body: 'value=' + i
                }).then(function() {
                    fetchStatus();
                    showToast('Effect: ' + effectNames[i]);
                });
            });
        })(i);
        fx.appendChild(cd);
    }
}

function updateWifiStatus(data) {
    var status = document.getElementById('wifiStatus');
    var section = document.getElementById('wifiSection');
    if (data.apMode) {
        status.textContent = 'Currently in AP Mode - Connect to configure WiFi';
        status.className = 'wifi-status ap';
        section.style.display = 'block';
    } else {
        status.textContent = 'Connected to: ' + (data.ssid || 'Unknown');
        status.className = 'wifi-status connected';
        section.style.display = 'block';
    }
}

function saveWifi() {
    if (!checkAuth()) return;
    var ssid = document.getElementById('wifiSsid').value.trim();
    var password = document.getElementById('wifiPassword').value;
    if (!ssid) {
        showToast('Please enter WiFi SSID');
        return;
    }
    if (confirm('Save WiFi settings and restart device?')) {
        fetch('/setWifi', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
                'Authorization': 'Bearer ' + localStorage.getItem('parola_token')
            },
            body: 'ssid=' + encodeURIComponent(ssid) + '&password=' + encodeURIComponent(password)
        }).then(function(r) {
            if (r.ok) {
                showToast('WiFi settings saved! Device restarting...');
                setTimeout(function() {
                    window.location.reload();
                }, 3000);
            } else {
                showToast('Failed to save WiFi settings');
            }
        }).catch(function() {
            showToast('Error saving WiFi settings');
        });
    }
}

// Event Listeners
document.addEventListener('DOMContentLoaded', function() {
    var msg = document.getElementById('msg');
    msg.addEventListener('input', function() {
        if (!checkAuth()) return;
        fetch('/setMessage', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
                'Authorization': 'Bearer ' + localStorage.getItem('parola_token')
            },
            body: 'value=' + encodeURIComponent(msg.value)
        });
        showToast('Live updated!');
    });

    setFxGallery();
    if (checkAuth()) {
        fetchStatus();
        setInterval(fetchStatus, 4000);
    }
    setPillSlider('bright', 5, BRIGHT_MIN, BRIGHT_MAX);
    setPillSlider('speed', 5, SPEED_MIN, SPEED_MAX);
    pillSliderHandler('bright', BRIGHT_MIN, BRIGHT_MAX);
    pillSliderHandler('speed', SPEED_MIN, SPEED_MAX);
});

document.getElementById('login-pop').onsubmit = function(e) {
    e.preventDefault();
    var u = document.getElementById('login-user').value.trim(),
        p = document.getElementById('login-pw').value;
    fetch('/login', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: 'username=' + encodeURIComponent(u) + '&password=' + encodeURIComponent(p)
    })
    .then(r => {
        if (!r.ok) throw new Error('Login failed');
        return r.json();
    })
    .then(data => {
        localStorage.setItem('parola_token', data.token);
        hideLogin();
        fetchStatus();
    })
    .catch(() => {
        document.getElementById('errbox').textContent = 'Incorrect username or password.';
    });
};

document.getElementById('reset-button').onclick = function(e) {
    e.preventDefault();
    if (!checkAuth()) return;
    if (confirm('Reset all settings and clear EEPROM?')) {
        fetch('/resetAll', {
            method: 'POST',
            headers: {
                'Authorization': 'Bearer ' + localStorage.getItem('parola_token')
            }
        }).then(res => {
            if (res.ok) {
                showToast('Reset done. Rebooting...');
                setTimeout(() => location.reload(), 3000);
            } else {
                showToast('Reset failed');
            }
        }).catch(() => showToast('Request failed'));
    }
};

document.getElementById('logout-link').onclick = function(e) {
    e.preventDefault();
    localStorage.removeItem('parola_token');
    showLogin();
};

document.getElementById('pweye').onmousedown = document.getElementById('pweye').ontouchstart = function() {
    var pwin = document.getElementById('login-pw');
    pwin.type = 'text';
    setTimeout(function() {
        pwin.type = 'password';
    }, 2200);
};

document.getElementById('pweye').onmouseup = document.getElementById('pweye').onmouseleave = document.getElementById('pweye').ontouchend = function() {
    document.getElementById('login-pw').type = 'password';
};

document.getElementById('otaBtn').onclick = function(e) {
    e.preventDefault();
    if (!checkAuth()) return;
    document.getElementById('binFile').click();
};

document.getElementById('binFile').onchange = function() {
    if (!checkAuth()) return;
    var fileInput = document.getElementById('binFile');
    var file = fileInput.files[0];
    if (!file) return;
    var xhr = new XMLHttpRequest();
    var form = new FormData();
    form.append('firmware', file);
    xhr.open('POST', '/update', true);
    xhr.setRequestHeader('Authorization', 'Bearer ' + localStorage.getItem('parola_token'));
    xhr.upload.onprogress = function(event) {
        if (event.lengthComputable) {
            var percent = Math.round((event.loaded / event.total) * 100);
            document.getElementById('fwBarPrg').style.width = percent + '%';
            document.getElementById('fwBarText').innerText = 'Uploading: ' + percent + '%';
            document.getElementById('fwLog').innerText = 'Uploading...';
        }
    };
    xhr.onload = function() {
        if (xhr.status == 200) {
            document.getElementById('fwBarPrg').style.width = '100%';
            document.getElementById('fwBarText').innerText = 'Upload complete!';
            document.getElementById('fwLog').innerText = 'Upload complete. Rebooting...';
            showToast('Firmware uploaded!');
        } else {
            document.getElementById('fwBarPrg').style.width = '0%';
            document.getElementById('fwBarText').innerText = 'Failed!';
            document.getElementById('fwLog').innerText = 'Update failed!';
            showToast('Update failed!');
        }
    };
    xhr.onerror = function() {
        document.getElementById('fwBarPrg').style.width = '0%';
        document.getElementById('fwBarText').innerText = 'Error!';
        document.getElementById('fwLog').innerText = 'Upload failed (network error)!';
        showToast('Upload failed!');
    };
    document.getElementById('fwBarPrg').style.width = '0%';
    document.getElementById('fwBarText').innerText = '0%';
    document.getElementById('fwLog').innerText = 'Preparing upload...';
    xhr.send(form);
};

// Initialize authentication check
if (!checkAuth()) {
    showLogin();
} else {
    hideLogin();
}
