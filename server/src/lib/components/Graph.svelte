<script lang="ts">
    import type { TemperatureData } from "$lib/model";

    let {
        array,
        min,
        max,
    }: { array: TemperatureData[]; min: number; max: number } = $props();

    function positionFromData(data: number, i: number): [number, number] {
        return [
            100 - ((100 / array.length) * i + 2.5),
            100 - (data / (max - min)) * 100,
        ];
    }
</script>

<div class="graph">
    <div
        class="ambient-line"
        style:top="{100 - (25 / (max - min)) * 100}%"
    ></div>
    {#each array as { data }, i}
        {@const position = positionFromData(data, i)}
        <div
            class="point"
            style:left="{position[0]}%"
            style:top="{position[1]}%"
        ></div>
    {/each}
    {#each array as { data }, i}
        {@const position = positionFromData(data, i)}
        <span style:left="{position[0]}%" style:top="{position[1]}%">
            {data}
        </span>
    {/each}
    <svg>
        {#each array.slice(0, -1) as { data }, i}
            {@const next = array[i + 1]}
            {@const position = positionFromData(data, i)}
            {@const nextPosition = positionFromData(next.data, i + 1)}
            <line
                x1="{position[0]}%"
                x2="{nextPosition[0]}%"
                y1="{position[1]}%"
                y2="{nextPosition[1]}%"
            ></line>
        {/each}
    </svg>
</div>

<style>
    .graph {
        position: relative;
        height: 10rem;
        background-color: var(--dark-1);
    }

    svg {
        position: absolute;
        left: 0;
        top: 0;
        width: 100%;
        height: 100%;

        line {
            stroke: var(--accent);
            stroke-width: 0.125rem;
            opacity: 0.5;
        }
    }

    span {
        position: absolute;
        left: 0;
        top: 0;
        transform: translate(-50%, -150%);
        font-size: 0.75rem;
    }

    .point {
        --size: 0.5rem;
        position: absolute;
        width: var(--size);
        height: var(--size);
        background-color: var(--accent);
        border-radius: calc(var(--size) / 2);
        transform: translate(-50%, -50%);
    }

    .ambient-line {
        position: absolute;
        left: 0;
        top: 50%;
        width: 100%;
        height: 0.125rem;
        transform: translateY(-50%);
        background-color: var(--accent);
        opacity: 0.125;
    }
</style>
