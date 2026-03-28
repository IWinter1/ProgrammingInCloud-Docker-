async function updateStatus() {
    try {
        // Pobranie naglowka z serwera http
        const response = await fetch(window.location.href, { method: 'HEAD' });
        
        // Pobranie danych z nginx
        const ip = response.headers.get('X-Server-IP');
        const host = response.headers.get('X-Server-Hostname');
        
        // Pobranie danych z pliku tekstowego
        const responseFile = await fetch('version.txt');
        const version = await responseFile.text();

        document.getElementById("ip").textContent = ip || "Error";
        document.getElementById("host").textContent = host || "Error";
        document.getElementById("ver").textContent = version.trim() || "Error";
    } catch (e) {
        console.error("Error while fetching data", e);
    }
}

document.addEventListener("DOMContentLoaded", updateStatus);