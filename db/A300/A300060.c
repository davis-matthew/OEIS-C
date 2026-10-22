/*
 * A300060  Number of domino tilings of the diagram of the integer partition
 *          with Heinz number n.
 * https://oeis.org/A300060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of domino tilings of the Young diagram of the
 *            integer partition whose Heinz number is n
 * Range:     u8 n <= 1680, u16 n <= 7920, u32 n <= 9369, u64 n <= 9369,
 *            u128 n <= 9369, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* prime handling ----------------------------------------------------------- */
static int primes[1500];
static int prime_cnt = 0;
static unsigned short prime_index_map[10001] = {0};
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    const int limit = 10000;
    unsigned char is_prime[10001];
    memset(is_prime, 1, sizeof(is_prime));
    is_prime[0] = is_prime[1] = 0;
    for (int p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (int q = p * p; q <= limit; q += p)
                is_prime[q] = 0;
        }
    }
    prime_cnt = 0;
    for (int i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            primes[prime_cnt++] = i;
            prime_index_map[i] = (unsigned short)(prime_cnt);     /* 1‑based index */
        }
    }
    primes_inited = true;
}

/* tiling of a single column ---------------------------------------------- */
static void dfs_fill(int pos, int h, int currMask, int nextMask,
                     seq_t val, seq_t *next_counts)
{
    while (pos < h && ((currMask >> pos) & 1)) pos++;
    if (pos == h) {
        next_counts[nextMask] += val;
        return;
    }
    /* vertical domino */
    if (pos + 1 < h && ((currMask >> (pos + 1)) & 1) == 0) {
        int newMask = currMask | (1 << pos) | (1 << (pos + 1));
        dfs_fill(pos + 2, h, newMask, nextMask, val, next_counts);
    }
    /* horizontal domino */
    int newMask2 = currMask | (1 << pos);
    dfs_fill(pos + 1, h, newMask2, nextMask | (1 << pos), val, next_counts);
}

/* main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* outside sequence scope, guard */
    /* factor n */
    uint64_t pf[64];
    uint64_t ex[64];
    size_t pc;
    factor_u64(n, pf, ex, &pc);          /* provided helper */

    init_primes();

    /* build partition parts (indices of primes) */
    int parts[64];
    size_t part_cnt = 0;
    for (size_t i = 0; i < pc; ++i) {
        unsigned short idx = prime_index_map[(unsigned int)pf[i]];
        for (uint64_t j = 0; j < ex[i]; ++j)
            parts[part_cnt++] = idx;
    }

    if (part_cnt == 0) return (seq_t)1;  /* empty diagram */

    /* compute width (max part size) and column heights */
    int width = 0;
    for (size_t i = 0; i < part_cnt; ++i)
        if (parts[i] > width) width = parts[i];

    int *heights = (int *)calloc(width, sizeof(int));
    for (size_t i = 0; i < part_cnt; ++i)
        for (int j = 0; j < parts[i]; ++j)
            heights[j]++;

    int maxHeight = (int)part_cnt;          /* number of rows */
    size_t masks = 1ULL << maxHeight;
    seq_t *cur = (seq_t *)calloc(masks, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(masks, sizeof(seq_t));
    cur[0] = (seq_t)1;

    for (int col = 0; col < width; ++col) {
        int h = heights[col];
        memset(nxt, 0, masks * sizeof(seq_t));

        for (size_t mask = 0; mask < masks; ++mask) {
            seq_t ways = cur[mask];
            if (!ways) continue;
            /* discard masks that touch non‑existent cells in this column */
            if (h >= 0 && (mask & ~((h == 0 ? 0ULL : ((1ULL << h) - 1))))) continue;
            dfs_fill(0, h, (int)mask, 0, ways, nxt);
        }
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    seq_t result = cur[0];   /* must end with no pending horizontal domino */

    free(heights);
    free(cur);
    free(nxt);
    return result;
}
