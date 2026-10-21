/*
 * A268335  Exponentially odd numbers.
 * https://oeis.org/A268335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 183, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   only_odd_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates all positive integers whose prime
 * factorisation contains only odd exponents (including exponent 1).
 * It searches successively larger integer bounds until it has found
 * at least n terms and then returns the (n‑1)th element (a(1) = 1).
 * Complexity is O(limit log log limit) per search, which is
 * comfortably below the 2.0 s limit for n ≤ 10 000. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/only_odd_exp.h"

/* ------------------------------------------------------------------
 * Helper: build an array spf[0..limit] where spf[x] is the smallest
 * prime factor of x (for x ≥ 2).  The array is allocated by the caller
 * and must be at least (limit+1)*sizeof(idx_t) bytes.
 * ------------------------------------------------------------------ */
static void build_spf(idx_t limit, idx_t *spf)
{
    /* initialise each entry to itself */
    for (idx_t i = 0; i <= limit; ++i) {
        spf[i] = i;
    }
    if (limit < 2) return;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] != i) continue;                 /* i not prime */
        for (idx_t j = i * i; j <= limit; j += i) {
            if (spf[j] == j) spf[j] = i;
        }
    }
}

/* ------------------------------------------------------------------
 * Main function required by the test harness.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initial bound is conservative; doubles until enough terms are found */
    idx_t bound = n * 2 + 100;                       /* start reasonably high */
    while (1) {
        /* allocate spf array */
        idx_t *spf = (idx_t *)malloc((size_t)(bound + 1) * sizeof(idx_t));
        if (!spf) return 0;                          /* allocation failure */

        build_spf(bound, spf);

        /* allocate result list */
        seq_t *list = (seq_t *)malloc((size_t)(bound + 1) * sizeof(seq_t));
        if (!list) {
            free(spf);
            return 0;
        }
        size_t count = 0;

        for (idx_t i = 1; i <= bound; ++i) {
            if (only_odd_exp(i, spf)) {
                if (count < (size_t)(bound + 1)) {
                    list[count++] = (seq_t)i;
                }
                if (count >= (size_t)n) break;
            }
        }

        free(spf);

        if (count >= (size_t)n) {
            seq_t result = list[n - 1];
            free(list);
            return result;
        }

        free(list);
        /* not enough terms – increase bound and try again */
        bound = bound * 2 + 100;
    }
}
