/*
 * A359002  Number of undirected n-cycles of the icosahedral graph.
 * https://oeis.org/A359002
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of undirected simple cycles of length n * in the
 *            icosahedral graph. a(3)=20, a(4)=30, …
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* adjacency list for the icosahedral graph (12 vertices, 5‑regular) */
static const unsigned char adj[12][5] = {
    {2, 4, 6, 8, 10}, /* 0 */
    {3, 4, 6, 9, 11}, /* 1 */
    {0, 5, 7, 8, 10}, /* 2 */
    {1, 5, 7, 9, 11}, /* 3 */
    {0, 1, 6, 8, 9},  /* 4 */
    {2, 3, 7, 8, 9},  /* 5 */
    {0, 1, 4,10,11},  /* 6 */
    {2, 3, 5,10,11},  /* 7 */
    {0, 2, 4, 5, 9},  /* 8 */
    {1, 3, 4, 5, 8},  /* 9 */
    {0, 2, 6, 7,11},  /*10 */
    {1, 3, 6, 7,10}   /*11 */
};

/* recursive search for directed simple cycles of length len,
 * starting from vertex 'start' (which is the smallest vertex in the cycle).
 */
static void dfs_cycle(int u, int depth, int len, int start,
                      bool *visited, seq_t *cnt)
{
    if (depth == len) {
        /* check whether we can close the cycle to the start */
        const unsigned char *neigh = adj[u];
        for (int i = 0; i < 5; ++i)
            if (neigh[i] == start) {
                ++(*cnt);
                break;
            }
        return;
    }

    const unsigned char *neigh = adj[u];
    for (int i = 0; i < 5; ++i) {
        int v = neigh[i];
        if (v == start || visited[v] || v < start)
            continue;                    /* keep start minimal, avoid repeats */
        visited[v] = true;
        dfs_cycle(v, depth + 1, len, start, visited, cnt);
        visited[v] = false;
    }
}

seq_t a(idx_t n)
{
    if (n < 3 || n > 12)
        return 0;

    static seq_t cache[13];
    static bool ready = false;

    if (!ready) {                      /* compute each term only once */
        for (int len = 3; len <= 12; ++len) {
            seq_t directed = 0;
            for (int s = 0; s < 12; ++s) {
                bool visited[12] = {false};
                visited[s] = true;
                dfs_cycle(s, 1, len, s, visited, &directed);
            }
            cache[len] = directed / (seq_t)2;   /* undo orientation factor */
        }
        ready = true;
    }

    return cache[n];
}
