/*
 * A376423  Nonnegative numbers m such that the run lengths in binary
 *          expansion of m, say (r_1, ..., r_k), correspond to a complete
 *          ruler: the sums r_i + ... r_j with i <= j <= k cover an initial
 *          interval of the positive integers.
 * https://oeis.org/A376423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A376423: non‑negative integers m whose binary run‑
 * lengths form a complete ruler.  It searches for the terms, caches them,
 * and returns the n‑th term (n ≥ 1).  The caching logic works for any
 * seq_t provided, provided seq_t can be constructed from a uint64_t by a
 * simple cast ((seq_t)uint64_t).  The code is valid for C11 and for C++17
 * if casted appropriately. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Determine whether m satisfies the complete ruler condition. */
static inline bool good_value(idx_t m)
{
    if (m == 0)                /* 0 is accepted by definition */
        return true;

    /* Find the highest set bit (index of most significant set bit). */
    int msb = 0;
    uint64_t tmp = m;
    while (tmp >>= 1)
        ++msb;

    /* Gather run lengths.  The number of runs is at most msb+1 ≤ 65. */
    int runs[65];
    int k = 0;
    int runlen = 1;
    bool prev_bit = (m >> msb) & 1U;

    for (int i = msb - 1; i >= 0; --i) {
        bool bit = (m >> i) & 1U;
        if (bit == prev_bit)
            ++runlen;
        else {
            runs[k++] = runlen;
            runlen = 1;
            prev_bit = bit;
        }
    }
    runs[k++] = runlen;        /* last run */

    int N = msb + 1;           /* total number of bits in m */

    /* present[s] == 1 if some contiguous sum of runs equals s. */
    unsigned char present[66];
    memset(present, 0, sizeof(present));

    for (int i = 0; i < k; ++i) {
        int sum = 0;
        for (int j = i; j < k; ++j) {
            sum += runs[j];
            if (sum <= N)
                present[sum] = 1;
        }
    }

    for (int s = 1; s <= N; ++s)
        if (!present[s])
            return false;
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache state. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static idx_t next_m = 0;

    if (n == 0)          /* out of range; return 0 as a harmless value */
        return 0;

    /* Generate terms until we reach the n‑th one. */
    while (cache_size < n) {
        if (good_value(next_m)) {
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                if (new_cap < n) new_cap = n;
                /* Cast the void* result of realloc to seq_t*. */
                seq_t *newp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
                if (!newp) abort();       /* unrecoverable failure */
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_m;
        }
        ++next_m;
    }
    return cache[n - 1];
}
