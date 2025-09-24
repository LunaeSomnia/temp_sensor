// src/routes/api/stream/+server.ts
import type { RequestHandler } from "./$types";
import {
    addClient,
    removeClient,
    sendData,
    type Controller,
} from "$lib/stream";
import { getLastData } from "$lib/db.svelte";
import type { TemperatureData } from "$lib/model";

export const GET: RequestHandler = async ({ request }) => {
    let thisController: Controller;
    const stream = new ReadableStream({
        async start(controller) {
            thisController = controller;
            addClient(controller);
            const lastData = await getLastData();
            sendData(controller, lastData);
        },
        cancel() {
            removeClient(thisController);
        },
    });

    return new Response(stream, {
        headers: {
            "Content-Type": "text/event-stream",
            "Cache-Control": "no-cache",
            Connection: "keep-alive",
        },
    });
};
