import { getLastData, insertData } from "$lib/db.svelte";
import type { TemperatureData } from "$lib/model";
import { broadcast } from "$lib/stream";
import type { RequestHandler } from "./$types";

export const POST: RequestHandler = async ({ request }) => {
    let newData: TemperatureData = await request.json();
    await insertData(newData);

    const lastData = await getLastData();
    broadcast(lastData);

    return new Response("Ok", { status: 200 });
};
