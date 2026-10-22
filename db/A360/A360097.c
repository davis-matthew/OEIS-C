/*
 * A360097  a(n) = smallest k such that 2*n*k-1 and 2*n*k+1 are nonprimes.
 * https://oeis.org/A360097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest k such that 2*n*k-1 and 2*n*k+1 are nonprime. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; n==0 is out of range but return 0 safely. */
    if (n == 0) return 0;

    seq_t n_seq = (seq_t)n;

    for (idx_t k = 1; ; ++k) {
        seq_t val = (seq_t)2 * n_seq * (seq_t)k;    /* 2*n*k */
        seq_t a1  = val - (seq_t)1;
        seq_t a2  = val + (seq_t)1;

        if (!is_prime_big(a1) && !is_prime_big(a2))
            return (seq_t)k;
    }
}
