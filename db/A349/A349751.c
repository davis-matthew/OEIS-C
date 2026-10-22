/*
 * A349751  Odd numbers k such that sigma(k) == -k (mod 3), where sigma is the
 *          sum of divisors function.
 * https://oeis.org/A349751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A349751:  a(n) is the nth odd integer k such that sigma(k)+k ≡ 0 (mod 3).  */
/*  Computation:  precompute sigma(k) for all integers up to a fixed limit, then collect
    odd k satisfying the congruence.  The implementation keeps the data in static
    arrays, so repeated calls to a() are fast.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Maximum number to pre‑compute sigma for.  This value is chosen to be large
   enough to hold at least the first 10 000 terms of the sequence. */
#define PRECOMP_LIMIT 2000000U

/* Helper to append a value to a dynamic array. */
static void append_value(seq_t **arr, size_t *len, size_t *cap, seq_t val) {
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 32 : (*cap * 2);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) { /* handle allocation failure */
            abort();
        }
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[(*len)++] = val;
}

/* Main function required by the build. */
seq_t a(idx_t n) {
    /* Static data structures: sigma array and the list of valid odd numbers. */
    static seq_t *sigma = NULL;
    static seq_t *valid = NULL;
    static size_t valid_len = 0;
    static size_t valid_cap = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Allocate and initialise sigma array. */
        sigma = (seq_t *)calloc((size_t)PRECOMP_LIMIT + 1, sizeof(seq_t));
        if (!sigma) abort();

        for (idx_t i = 1; i <= PRECOMP_LIMIT; ++i) {
            seq_t ii = (seq_t)i;
            for (idx_t j = i; j <= PRECOMP_LIMIT; j += i) {
                sigma[j] += ii;
            }
        }

        /* Collect odd numbers satisfying sigma(k)+k ≡ 0 (mod 3). */
        for (idx_t k = 1; k <= PRECOMP_LIMIT; k += 2) {
            seq_t kk = (seq_t)k;
            if ((sigma[k] + kk) % 3 == 0) {
                append_value(&valid, &valid_len, &valid_cap, kk);
            }
        }

        /* Free the sigma array as it is no longer needed. */
        free(sigma);
        sigma = NULL;

        initialized = true;
    }

    /* Return the requested term.  The caller guarantees n ≥ 1. */
    return valid[n - 1];
}
