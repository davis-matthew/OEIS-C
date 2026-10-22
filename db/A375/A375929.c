/*
 * A375929  Numbers k such that A002808(k+1) = A002808(k) + 1. In other words,
 *          the k-th composite number is 1 less than the next.
 * https://oeis.org/A375929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   build_prime_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375929:
   a(n) is the position k of the k‑th composite number such that the
   next composite A002808(k+1) differs from A002808(k) by 1.
   The value returned is the index of the composite number within
   the sequence of all composites.                           */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_prime_sieve.h"

/* ------------------------------------------------------------------ */
/* Helper: sieve primes up to a given limit.                         */

/* ------------------------------------------------------------------ */
/* Main function required by the build.                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety: a(0) is undefined */

    /* static cache of prime flags and maximum limit used so far */
    static bool *prime_flags = NULL;
    static idx_t max_limit = 0;

    /* compute a rough initial limit that should contain at least n terms
       (for n up to 10000 this works, for larger n the loop below will
       enlarge the limit if necessary).                        */
    idx_t desired_limit = n * 15 + 1000;
    if (desired_limit < 100) desired_limit = 100;

    /* reallocate and rebuild the sieve only if we need a larger limit */
    if (desired_limit > max_limit) {
        bool *new_flags = (bool *)calloc((size_t)desired_limit + 2, sizeof(bool));
        if (!new_flags) return 0; /* out of memory */
        build_prime_sieve(desired_limit, new_flags);
        /* free old array if it existed */
        if (prime_flags) free(prime_flags);
        prime_flags = new_flags;
        max_limit = desired_limit;
    }

    /* Scan numbers to find the n-th qualifying composite.           */
    idx_t composite_index = 0;   /* position of current composite in A002808 */
    idx_t found = 0;
    seq_t answer = 0;

    for (idx_t num = 4; num <= max_limit; ++num) {
        if (!prime_flags[num]) {                /* num is composite */
            ++composite_index;
            if (num + 1 <= max_limit && !prime_flags[num + 1]) {
                ++found;
                if (found == n) {
                    answer = (seq_t)composite_index;
                    return answer;
                }
            }
        }
    }

    /* If we reached here, the current limit was insufficient.
       Increase the limit and continue searching.                */
    while (found < n) {
        idx_t new_limit = max_limit * 2 + 1000;
        bool *new_flags = (bool *)calloc((size_t)new_limit + 2, sizeof(bool));
        if (!new_flags) return 0; /* out of memory */
        build_prime_sieve(new_limit, new_flags);
        if (prime_flags) free(prime_flags);
        prime_flags = new_flags;
        max_limit = new_limit;

        /* restart scanning from the beginning of the new range */
        composite_index = 0;
        found = 0;
        for (idx_t num = 4; num <= max_limit; ++num) {
            if (!prime_flags[num]) {
                ++composite_index;
                if (num + 1 <= max_limit && !prime_flags[num + 1]) {
                    ++found;
                    if (found == n) {
                        answer = (seq_t)composite_index;
                        return answer;
                    }
                }
            }
        }
    }

    /* Should never reach here */
    return 0;
}
