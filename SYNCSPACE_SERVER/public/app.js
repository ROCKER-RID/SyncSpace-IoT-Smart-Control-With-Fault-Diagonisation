// Connect to the server
const socket = io();

// Connection status handling
socket.on('connect', () => {
    updateConnectionStatus(true);
});

socket.on('disconnect', () => {
    updateConnectionStatus(false);
});

socket.on('connection_status', (data) => {
    updateConnectionStatus(data.connected);
});

function updateConnectionStatus(connected) {
    const statusEl = document.getElementById('connectionStatus');
    const textEl = document.getElementById('connectionText');
    
    if (connected) {
        statusEl.className = 'status connected';
        textEl.textContent = 'Connected';
    } else {
        statusEl.className = 'status disconnected';
        textEl.textContent = 'Disconnected';
    }
}

// Update UI with device state
socket.on('state_update', (state) => {
    updateRelayUI(state.relays);
    updateFanUI(state.fans);
    updateZCDInfo(state.zcd_interval);
});

// Initialize UI
function initializeUI() {
    const relayControls = document.getElementById('relayControls');
    const fanControls = document.getElementById('fanControls');
    
    // Create relay controls
    for (let i = 0; i < 5; i++) {
        const relayDiv = document.createElement('div');
        relayDiv.className = 'control-item';
        relayDiv.innerHTML = `
            <h4>Relay ${i+1}</h4>
            <button id="relay-${i}" class="off" onclick="toggleRelay(${i})">OFF</button>
        `;
        relayControls.appendChild(relayDiv);
    }
    
    // Create fan controls
    for (let i = 0; i < 3; i++) {
        const fanDiv = document.createElement('div');
        fanDiv.className = 'control-item';
        fanDiv.innerHTML = `
            <h4>Fan ${i+1}</h4>
            <div class="slider-container">
                <input type="range" id="fan-${i}" class="slider" 
                       min="0" max="100" value="0" 
                       oninput="updateFanSpeed(${i}, this.value)">
                <span id="fan-${i}-value">0%</span>
            </div>
        `;
        fanControls.appendChild(fanDiv);
    }
}

// Update relay UI based on state
function updateRelayUI(relays) {
    relays.forEach(relay => {
        const button = document.getElementById(`relay-${relay.relay}`);
        if (button) {
            if (relay.state) {
                button.textContent = 'ON';
                button.className = '';
            } else {
                button.textContent = 'OFF';
                button.className = 'off';
            }
        }
    });
}

// Update fan UI based on state
function updateFanUI(fans) {
    fans.forEach(fan => {
        const slider = document.getElementById(`fan-${fan.fan}`);
        const valueDisplay = document.getElementById(`fan-${fan.fan}-value`);
        
        if (slider && valueDisplay) {
            slider.value = fan.speed;
            valueDisplay.textContent = `${fan.speed}%`;
        }
    });
}

// Update ZCD information
function updateZCDInfo(zcdInterval) {
    const zcdInfo = document.getElementById('zcdInfo');
    zcdInfo.textContent = `ZCD Interval: ${zcdInterval} µs`;
}

// Send relay toggle command
function toggleRelay(relay) {
    const button = document.getElementById(`relay-${relay}`);
    const newState = button.textContent === 'OFF';
    
    socket.emit('command', {
        relay: relay,
        state: newState
    });
}

// Send fan speed update command
function updateFanSpeed(fan, speed) {
    document.getElementById(`fan-${fan}-value`).textContent = `${speed}%`;
    
    // Debounce to avoid too many events
    clearTimeout(window.fanDebounce);
    window.fanDebounce = setTimeout(() => {
        socket.emit('command', {
            fan: fan,
            speed: parseInt(speed)
        });
    }, 100);
}

// Initialize UI when document is ready
document.addEventListener('DOMContentLoaded', initializeUI);