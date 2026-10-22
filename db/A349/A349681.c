/*
 * A349681  a(1)=1, a(2)=2; for n > 2, a(n) is the least unused positive
 *          number k such that j != k and either j | k or k | j, but not both,
 *          where j = a(n-1) + 1.
 * https://oeis.org/A349681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 421, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A349681
 * a(1)=1, a(2)=2; for n>2,
 * let j = a(n‑1)+1.
 * a(n) is the least unused positive integer k with j≠k and exactly
 * one of j|k or k|j holds (i.e. k is a proper divisor or a proper multiple
 * of j).
 *
 * The algorithm maintains a dynamic boolean array 'used' marking all
 * numbers that have appeared in the sequence.  For each step it
 *   • examines all proper divisors of j by trial division up to √j,
 *   • searches upward from 2·j for the first unused proper multiple,
 *   • and returns the smaller of the two candidates.
 *
 * Complexity: roughly O(√j) per term; fast enough for n≈10000, and
 * memory bounded by the largest value reached.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Grow a boolean array if an index is required beyond its current capacity. */
static void ensure_used_cap(size_t needed, bool **used, size_t *used_cap)
{
    if (needed < *used_cap)
        return;                     /* already big enough */
    size_t new_cap = (*used_cap > 0 ? *used_cap : 4);
    while (new_cap <= needed)
        new_cap <<= 1;               /* double until we fit */
    bool *new_arr = (bool *)realloc(*used, new_cap * sizeof(bool));
    if (!new_arr)
        abort();                    /* out of memory */
    /* initialise the newly allocated entries as unused */
    for (size_t i = *used_cap; i < new_cap; ++i)
        new_arr[i] = false;
    *used = new_arr;
    *used_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static data structures for memoisation */
    static seq_t *seq   = NULL;
    static size_t seq_cap = 0, seq_len = 0;
    static bool *used   = NULL;
    static size_t used_cap = 0;

    /* first call initialisation */
    if (seq_len == 0) {
        /* base sequence table */
        seq_cap = 4;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq)
            abort();
        seq[0] = (seq_t)1;          /* a(1) */
        seq[1] = (seq_t)2;          /* a(2) */
        seq_len = 2;

        /* boolean used array (index 0 unused) */
        used_cap = 4;
        used = (bool *)malloc(used_cap * sizeof(bool));
        if (!used)
            abort();
        for (size_t i = 0; i < used_cap; ++i)
            used[i] = false;
        used[1] = true;
        used[2] = true;
    }

    /* if n already computed, just return it */
    if ((idx_t)n <= seq_len)
        return seq[(size_t)n - 1];

    /* grow sequence array if needed */
    if ((size_t)n > seq_cap) {
        size_t new_cap = seq_cap;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!new_seq)
            abort();
        seq = new_seq;
        seq_cap = new_cap;
    }

    /* compute subsequent terms up to n */
    for (size_t idx = seq_len; idx < (size_t)n; ++idx) {
        seq_t j = seq[idx - 1] + (seq_t)1;        /* j = a(n-1)+1 */

        /* cast j to 64‑bit for integer arithmetic (safe for the range of n we handle) */
        uint64_t uj = (uint64_t)j;

        /* 1) find smallest unused proper divisor of j */
        size_t min_div = SIZE_MAX;
        for (uint64_t d = 2; d * d <= uj; ++d) {
            if (uj % d == 0) {                    /* d is a divisor */
                size_t cand1 = (size_t)d;
                if (!used[cand1] && cand1 < min_div)
                    min_div = cand1;
                size_t cand2 = (size_t)(uj / d);
                if (cand2 != uj && !used[cand2] && cand2 < min_div)
                    min_div = cand2;
                if (min_div == 2)                 /* can't get smaller */
                    break;
            }
        }
        /* 2) find smallest unused proper multiple of j (start at 2·j) */
        size_t min_mul = SIZE_MAX;
        {
            /* first candidate: j * 2 */
            uint64_t m = uj * 2ULL;
            size_t idx_of_m;
            for (;;) {
                /* ensure array large enough for this candidate */
                ensure_used_cap(m, &used, &used_cap);
                idx_of_m = (size_t)m;
                if (!used[idx_of_m]) {
                    min_mul = idx_of_m;
                    break;
                }
                m += uj;                         /* next multiple */
            }
        }
        /* choose the smaller of the two candidates */
        size_t chosen = (min_div < min_mul) ? min_div : min_mul;

        seq[idx] = (seq_t)chosen;
        used[chosen] = true;
    }

    seq_len = (size_t)n;
    return seq[(size_t)n - 1];
}
