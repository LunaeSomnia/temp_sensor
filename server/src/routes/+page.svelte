<script lang="ts">
    import Card from "$lib/components/Card.svelte";
    import Graph from "$lib/components/Graph.svelte";
    import type { TemperatureData } from "$lib/model";
    import { onDestroy } from "svelte";

    let readings: TemperatureData[] = $state([]);

    const source = new EventSource("/api/stream");
    source.onmessage = (event) => {
        readings = JSON.parse(event.data);
    };

    onDestroy(() => {
        source.close();
    });

    function dateFromTimestamp(timestamp: number) {
        let date = new Date(0);
        date.setUTCSeconds(timestamp);
        return date;
    }
</script>

{#if readings && readings[0] && readings[0].data}
    {@const dateString = dateFromTimestamp(readings[0].timestamp)}
    <Card>
        <p>
            {dateString.getDate()}/{dateString.getMonth()}/{dateString.getFullYear()}
        </p>
        <p>Current temperature:</p>
        <h1>{readings[0].data}ºC</h1>
    </Card>
{/if}
<Card>
    <Graph array={readings} min={-5} max={55} />
</Card>

<Card>
    <ul>
        {#each readings as r}
            <li>{dateFromTimestamp(r.timestamp)}: {r.data} °C</li>
        {/each}
    </ul>
</Card>
