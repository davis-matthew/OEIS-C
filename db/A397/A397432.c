/*
 * A397432  Composites in A161601 whose binary reversal is composite.
 * https://oeis.org/A397432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th composite integer m such that revbin(m) > m and
 *            revbin(m) is also composite, where revbin reverses the binary
 *            representation of its argument
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   rev_bin_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/rev_bin_idx.h"

/* Simple trial‑division primality test for 64‑bit integers. */
static inline bool is_prime_idx(idx_t v)
{
    if (v < 2)          return false;
    if (v % 2 == 0)     return v == 2;
    for (idx_t d = 3; d <= v / d; d += 2) {
        if (v % d == 0) return false;
    }
    return true;
}

/* Compute the n-th term of A397432.  The function is order‑independent
   and works for any n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined below offset 1 */
    idx_t count = 0;
    for (idx_t x = 4; ; ++x) {         /* start above the first composite */
        /* Skip primes. */
        if (is_prime_idx(x)) continue;
        /* Compute the binary reversal of x. */
        idx_t rev = rev_bin_idx(x);
        if (rev <= x) continue;       /* need rev > x */
        /* Skip if the reversal is prime. */
        if (is_prime_idx(rev)) continue;
        ++count;
        if (count == n) return (seq_t)x;
    }
}
