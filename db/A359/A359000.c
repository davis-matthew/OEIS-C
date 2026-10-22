/*
 * A359000  Number of undirected n-cycles of the octahedral graph.
 * https://oeis.org/A359000
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of undirected n‑cycles in the octahedral graph
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   canonical_key
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n > 6 there are no such cycles, so a(n) = 0. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/canonical_key.h"

static const int opp[6] = {5, 4, 3, 2, 1, 0};

static bool next_permutation(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) i--;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] <= a[i]) j--;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    /* reverse a[i+1..n-1] */
    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
        l++; r--;
    }
    return true;
}

static seq_t count_cycles(int n)
{
    int vertices[6] = {0, 1, 2, 3, 4, 5};
    seq_t total = 0;
    uint64_t keys[32];
    int key_count = 0;

    for (int mask = 0; mask < (1 << 6); ++mask) {
        if (__builtin_popcount((unsigned)mask) != (unsigned)n) continue;
        int subset[6];
        int sz = 0;
        for (int v = 0; v < 6; ++v)
            if (mask & (1 << v)) subset[sz++] = v;   /* preserves ascending order */

        /* generate all permutations of the subset */
        do {
            bool good = true;
            for (int i = 0; i < n; ++i) {
                int u = subset[i];
                int w = subset[(i + 1) % n];
                if (w == opp[u]) { good = false; break; }
            }
            if (!good) continue;

            uint64_t key = canonical_key(subset, n);
            bool seen = false;
            for (int k = 0; k < key_count; ++k)
                if (keys[k] == key) { seen = true; break; }
            if (!seen) {
                keys[key_count++] = key;
                ++total;
            }
        } while (next_permutation(subset, sz));
    }
    return total;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t cache[7] = {0};

    if (!initialized) {
        for (int i = 3; i <= 6; ++i)
            cache[i] = count_cycles(i);
        cache[0] = cache[1] = cache[2] = 0;
        initialized = true;
    }

    if (n > 6) return 0;
    return cache[(int)n];
}
