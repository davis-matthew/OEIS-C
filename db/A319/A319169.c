/*
 * A319169  Number of integer partitions of n whose parts all have the same
 *          number of prime factors, counted with multiplicity.
 * https://oeis.org/A319169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=0} p_k(n) where p_k(n) counts partitions of n all
 *            of whose parts have big‑omega (number of prime factors with
 *            multiplicity) equal to k. The offset is 0 (a(0)=1)
 * Range:     u8 n <= 37, u16 n <= 105, u32 n <= 348, u64 n <= 1248,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary headers for integer types and memory allocation */
#include <stdint.h>
#include <stdlib.h>

/* Forward declaration of helper function that recomputes the sequence
   up to a maximum index n. */
static seq_t *recompute(idx_t n);

/* Return a(n) with memoisation; O(n^2) time, O(n * maxOmega) memory. */
seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static idx_t cached_max_n   = 0;
    static seq_t *cached_a      = NULL;

    if (n <= cached_max_n && cached_a != NULL) {
        return cached_a[n];
    }

    /* Recompute sequence up to n and replace the cache. */
    seq_t *new_a = recompute(n);
    if (new_a == NULL) {     /* allocation failure – return 0 as a fallback */
        return (seq_t)0;
    }
    if (cached_a) {
        free(cached_a);
    }
    cached_a = new_a;
    cached_max_n = n;
    return cached_a[n];
}

/* ----------  Helper implementation ---------- */

/* Recompute a(0)..a(n) from scratch.  Frees all temporary memory. */
static seq_t *recompute(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0) {
        seq_t *arr = (seq_t*)calloc(1, sizeof(seq_t));
        if (arr) arr[0] = (seq_t)1;
        return arr;
    }

    size_t N = (size_t)n;                 /* assume n <= SIZE_MAX */

    /* ---- 1. Compute smallest prime factors (SPF) and big‑omega. ---- */
    uint32_t *spf   = (uint32_t*)calloc(N + 1, sizeof(uint32_t));
    uint8_t  *omega = (uint8_t *)calloc(N + 1, sizeof(uint8_t));
    if (!spf || !omega) {
        free(spf); free(omega);
        return NULL;
    }

    spf[0] = 0; spf[1] = 1;
    omega[0] = 0; omega[1] = 0;

    for (size_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {                      /* i is prime */
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= N) {
                for (uint32_t j = (uint32_t)(i * i); j <= N; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }

    uint8_t maxOmega = 0;
    for (size_t i = 2; i <= N; ++i) {
        omega[i] = omega[i / spf[i]] + 1;
        if (omega[i] > maxOmega) maxOmega = omega[i];
    }

    /* ---- 2. Allocate dynamic programming arrays for each Omega. ---- */
    seq_t **dp = (seq_t**)calloc(maxOmega + 1, sizeof(seq_t*));
    if (!dp) {
        free(spf); free(omega);
        return NULL;
    }
    for (uint8_t k = 0; k <= maxOmega; ++k) {
        dp[k] = (seq_t*)calloc(N + 1, sizeof(seq_t));   /* zero initialisation */
        if (!dp[k]) {
            for (uint8_t j = 0; j < k; ++j) free(dp[j]);
            free(dp); free(spf); free(omega);
            return NULL;
        }
        dp[k][0] = (seq_t)1;        /* empty partition counts as one */
    }

    /* ---- 3. Process each part m = 1..N. ---- */
    for (size_t m = 1; m <= N; ++m) {
        uint8_t k = omega[m];
        seq_t *curdp = dp[k];
        for (size_t v = m; v <= N; ++v) {
            curdp[v] += curdp[v - m];
        }
    }

    /* ---- 4. Compute final sequence values. ---- */
    seq_t *a_arr = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!a_arr) {
        for (uint8_t k = 0; k <= maxOmega; ++k) free(dp[k]);
        free(dp); free(spf); free(omega);
        return NULL;
    }

    for (size_t v = 1; v <= N; ++v) {          /* v = 0 handled later */
        seq_t val = (seq_t)0;
        for (uint8_t k = 0; k <= maxOmega; ++k) {
            val += dp[k][v];
        }
        a_arr[v] = val;
    }
    a_arr[0] = (seq_t)1;  /* offset 0: exactly one empty partition */

    /* ---- 5. Cleanup temporary memory. ---- */
    for (uint8_t k = 0; k <= maxOmega; ++k) {
        free(dp[k]);
    }
    free(dp);
    free(spf);
    free(omega);

    return a_arr;
}
