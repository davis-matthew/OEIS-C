/*
 * A257667  Primes containing a digit 5.
 * https://oeis.org/A257667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1595, u32 n <= 1596, u64 n <= 9624,
 *            u128 n <= 9624, big any n
 * Helpers:   append_candidate, contains_digit5, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257667: primes that contain a digit 5. */
/* The function searches all primes in increasing order and stores
   those that contain the digit 5 in a static cache. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"
#include "../utils/contains_digit5.h"

/* Return the n-th prime that contains a digit 5 (1‑based index). */
seq_t a(idx_t n) {
    /* Static cache of found primes. */
    static seq_t *cache    = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 0;   /* last integer examined */

    /* Sanity check: the OEIS sequence is defined for n >= 1. */
    if (n == 0) {
        return 0;   /* undefined, but avoid UB */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Move to the next integer. */
        last_checked = last_checked + 1;

        /* Test for prime and digit‑5 containment. */
        if (is_prime_big(last_checked) && contains_digit5(last_checked)) {
            append_candidate(&cache, &cache_len, &cache_cap, last_checked);
        }
    }

    return cache[(size_t)n - 1];
}
