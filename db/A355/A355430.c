/*
 * A355430  Primes starting with an even decimal digit.
 * https://oeis.org/A355430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime whose reversal is even (i.e., whose first
 *            decimal digit is even)
 * Range:     u8 n <= 18, u16 n <= 2945, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"
#include "../utils/rev.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Cache of previous terms */
    static seq_t *cache = NULL;      /* array of terms */
    static size_t cap   = 0;         /* capacity of cache */
    static size_t len   = 0;         /* number of terms stored */
    static idx_t next_trial = 2;    /* next integer to test */

    /* Search for new terms until we have at least n of them */
    idx_t cand;
    for (cand = next_trial; len < n; ++cand) {
        if (!is_prime(cand))
            continue;
        /* rev(cand) returns the decimal reversal of cand */
        /* If the reversal is even, the first decimal digit of cand is even */
        if ((rev(cand) & 1) != 0)
            continue;
        /* Append the prime as the next term */
        append_candidate(&cache, &len, &cap, (seq_t)cand);
    }

    /* Record the next integer to start searching from */
    next_trial = cand;

    /* cache[n‑1] now holds the requested term */
    return cache[n - 1];
}
