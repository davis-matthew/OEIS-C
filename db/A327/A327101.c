/*
 * A327101  BII-numbers of 2-cut-connected set-systems (cut-connectivity >=
 *          2).
 * https://oeis.org/A327101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) enumerates the BII-numbers of 2‑cut‑connected set‑systems.
 *            The value a(n) is found by testing successive integer candidates
 *            n >= 1 for the property described in OEIS A327101. The algorithm
 *            keeps a cache of found values so that repeated calls are
 *            answered in O(1) time after the initial enumer...
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Update: a correction of the connectivity check has been added.
   The original implementation considered a candidate valid if all
   vertex‑removals left a connected system, but it did not require
   the original system itself to be connected.  This caused an
   off‑by‑one error; the current version checks that the full
   set‑system is contiguous before testing removals. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- helpers ------------------------------------------------- */
static inline unsigned bitlen_u64(uint64_t x)
{
    return x ? 64 - __builtin_clzll(x) : 0;
}

/* compute binary indices of m: positions of 1 bits counted from LSB (1‑based) */
static void get_bin_indices(uint64_t m, int *out, int *cnt)
{
    int len = bitlen_u64(m);
    int k = 0;
    for (int j = 0; j < len; ++j)
        if ((m >> j) & 1ULL)
            out[k++] = j + 1;
    *cnt = k;
}

/* check connectivity of a hypergraph given by masks and the set of vertices
   present (tot_mask).  Returns true if the induced graph on these
   vertices is connected.  We build an adjacency mask for each vertex
   (bit i set if it is adjacent to the current vertex). */
static bool is_connected(uint64_t *masks, int cnt, uint64_t tot_mask)
{
    uint64_t adj[64] = {0};

    for (int i = 0; i < cnt; ++i) {
        uint64_t m = masks[i];
        while (m) {
            int u = __builtin_ctzll(m);          /* vertex u (0‑based) */
            uint64_t rest = m & ~(1ULL << u);    /* all other vertices in this set */
            adj[u] |= rest;
            m &= m - 1;
        }
    }

    /* find first present vertex */
    int start = -1;
    for (int u = 0; u < 64; ++u)
        if (tot_mask & (1ULL << u)) {
            start = u;
            break;
        }

    if (start == -1)  /* should never happen for a nonempty system */
        return false;

    uint64_t visited = 0;
    int q[64];
    int qh = 0, qt = 0;
    visited |= 1ULL << start;
    q[qt++] = start;

    while (qh < qt) {
        int cur = q[qh++];
        uint64_t neigh = adj[cur];
        uint64_t to = neigh & ~visited;
        while (to) {
            int v = __builtin_ctzll(to);
            visited |= 1ULL << v;
            q[qt++] = v;
            to &= to - 1;
        }
    }

    return (visited & tot_mask) == tot_mask;
}

/* determine if candidate is a BII‑number of a 2‑cut‑connected set‑system */
static bool is_bii_2cut(const uint64_t candidate)
{
    /* obtain binary indices of the candidate */
    int idx_arr[64];
    int idx_cnt = 0;
    get_bin_indices(candidate, idx_arr, &idx_cnt);
    if (idx_cnt == 0)
        return false;

    /* build the set‑system masks (each set is a vertex mask) */
    uint64_t masks[64];
    int mcnt = 0;
    for (int i = 0; i < idx_cnt; ++i) {
        uint64_t k = (uint64_t)idx_arr[i];
        int sub_arr[64];
        int sub_cnt = 0;
        get_bin_indices(k, sub_arr, &sub_cnt);
        if (sub_cnt == 0)
            continue;                       /* empty set would be removed automatically */
        uint64_t mask = 0;
        for (int j = 0; j < sub_cnt; ++j)
            mask |= 1ULL << (sub_arr[j] - 1);
        if (mask)
            masks[mcnt++] = mask;
    }
    if (mcnt == 0)
        return false;

    /* total vertex mask of the system */
    uint64_t tot_mask = 0;
    for (int i = 0; i < mcnt; ++i)
        tot_mask |= masks[i];
    if (tot_mask == 0)
        return false;

    /* the full system must be connected */
    if (!is_connected(masks, mcnt, tot_mask))
        return false;

    /* test removal of each vertex */
    for (int v = 0; v < 64; ++v) {
        if (!(tot_mask & (1ULL << v)))
            continue;                          /* vertex not present */

        uint64_t rem_masks[64];
        int rcnt = 0;
        uint64_t rem_tot = 0;
        for (int i = 0; i < mcnt; ++i) {
            uint64_t m2 = masks[i] & ~(1ULL << v);
            if (m2) {
                rem_masks[rcnt++] = m2;
                rem_tot |= m2;
            }
        }

        if (rcnt == 0)          /* empty set‑system after removal */
            return false;

        if (!is_connected(rem_masks, rcnt, rem_tot))
            return false;
    }

    return true;
}

/* ---------- caching mechanism -------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;  /* next integer to test */

/* resize the cache array */
static void grow_cache(size_t new_cap)
{
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) {
        /* allocation failed; abort as this is a critical error */
        abort();
    }
    cache = newp;
    cache_cap = new_cap;
}

/* extend the cache until it contains at least target many values */
static void extend_until(idx_t target)
{
    while (cache_size < target) {
        while (!is_bii_2cut(next_candidate))
            ++next_candidate;
        if (cache_size == cache_cap)
            grow_cache(cache_cap > 0 ? cache_cap * 2 : 4);
        cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* ---------- public API --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)    /* offset is 1; value undefined for n < 1 */
        return (seq_t)0;
    extend_until(n);
    return cache[n - 1];
}
