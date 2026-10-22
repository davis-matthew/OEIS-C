/*
 * A328434  Number of inversion sequences of length n avoiding the consecutive
 *          patterns 101, 102, 201, and 210.
 * https://oeis.org/A328434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328434 : Number of inversion sequences of length n avoiding
   the consecutive patterns 101,102,201,210.  The algorithm uses a
   two‑state DP in O(n²) time and O(n) memory.  For a sequence of
   length ℓ (ℓ≥1) let

      N[ℓ][v] – number of sequences of length ℓ whose last value is v
                and the last comparison (between positions ℓ‑1 and ℓ)
                is NOT a drop (e_{ℓ‑1} ≤ e_ℓ).

      D[ℓ][v] – number of sequences of length ℓ whose last value is v
                and the last comparison is a drop (e_{ℓ‑1} > e_ℓ).

   The transition from length ℓ to ℓ+1 is:
      • From N[v] :
            • for every w ≥ v    → N[w] increased by N[v]
            • for every w <  v   → D[w] increased by N[v]
      • From D[v] :
            • the next (forced) value is v → N[v] increased by D[v].

   The above updates are implemented efficiently by prefix sums.  The
   final value a(n) is the total number of sequences of length n,
   i.e. sum over all v of N[n][v] + D[n][v].  A cache of a(0)…a(n)
   guarantees that calls arrive in any order. */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    static idx_t computed_max = 0;        /* highest n already computed */
    static seq_t *cache = NULL;           /* a(0)…a(computed_max) */
    static size_t cache_cap = 0;
    static seq_t *cntN = NULL;            /* N counts for current length */
    static seq_t *cntD = NULL;            /* D counts for current length */

    /* ---------- initialization ---------- */
    if (computed_max == 0) {
        /* a(0) = 1 */
        cache_cap = 2;
        cache = (seq_t*) malloc(sizeof(seq_t) * cache_cap);
        cache[0] = 1;                      /* a(0) */
        cache[1] = 1;                      /* a(1) – single sequence [0] */

        /* length 1 arrays */
        cntN = (seq_t*) malloc(sizeof(seq_t) * 1);
        cntD = (seq_t*) malloc(sizeof(seq_t) * 1);
        cntN[0] = 1;   /* sequence [0] */
        cntD[0] = 0;
        computed_max = 1;
    }

    /* ---------- extend DP until we have a(n) ---------- */
    while (computed_max < n) {
        idx_t curLen = computed_max;          /* current length of DP arrays */
        idx_t newLen = curLen + 1;            /* length to compute */

        /* allocate next arrays */
        seq_t *nextN = (seq_t*) malloc(sizeof(seq_t) * newLen);
        seq_t *nextD = (seq_t*) malloc(sizeof(seq_t) * newLen);
        for (idx_t i = 0; i < newLen; ++i) {
            nextN[i] = 0;
            nextD[i] = 0;
        }

        /* total sum of cntN (equals a(curLen) - sum(cntD)) */
        seq_t totalSum = 0;
        for (idx_t i = 0; i < curLen; ++i) {
            totalSum += cntN[i];
        }

        /* nextN[curLen] gathers all contributions from N to w = curLen */
        nextN[curLen] = totalSum;

        /* prefix sum over cntN to compute updates in O(curLen) */
        seq_t prefSum = 0;
        for (idx_t w = 0; w < curLen; ++w) {
            prefSum += cntN[w];
            nextN[w] = prefSum + cntD[w];
            nextD[w] = totalSum - prefSum;     /* sum_{v> w} cntN[v] */
        }
        /* nextD[curLen] stays 0 (already zero) */

        /* compute a(newLen) = sum of nextN + nextD */
        seq_t newTotal = 0;
        for (idx_t i = 0; i < newLen; ++i) {
            newTotal += nextN[i] + nextD[i];
        }

        /* free old arrays */
        free(cntN);
        free(cntD);

        cntN = nextN;
        cntD = nextD;
        ++computed_max;

        /* grow cache if necessary */
        if (computed_max >= cache_cap) {
            cache_cap = cache_cap * 2 + 1;
            seq_t *tmp = (seq_t*) realloc(cache, sizeof(seq_t) * cache_cap);
            if (!tmp) { /* allocation failed, abort */
                free(cntN);
                free(cntD);
                free(cache);
                return 0;
            }
            cache = tmp;
        }
        cache[computed_max] = newTotal;
    }

    /* ---------- return cached value ---------- */
    return cache[n];
}
