<script lang="ts">
    let { children } = $props();
</script>

<div class="card">
    {@render children()}
</div>

<style>
    .card {
        padding: 1rem;
        background-color: var(--dark-2);
    }
</style>
