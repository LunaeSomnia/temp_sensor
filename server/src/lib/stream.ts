import type { TemperatureData } from "./model";

export type Controller = ReadableStreamDefaultController<string>;
const clients: Controller[] = [];

export function addClient(controller: Controller) {
    clients.push(controller);
}

export function removeClient(controller: Controller) {
    const i = clients.indexOf(controller);
    if (i !== -1) clients.splice(i, 1);
}

export function sendData(controller: Controller, data: TemperatureData[]) {
    const payload = JSON.stringify(data);
    controller.enqueue(`data: ${payload}\n\n`);
}

export function broadcast(data: TemperatureData[]) {
    for (const controller of clients) {
        sendData(controller, data);
    }
}
