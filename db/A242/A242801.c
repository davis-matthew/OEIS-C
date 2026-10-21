/*
 * A242801  Least number k > 1 such that (k^k+n)/(k+n) is an integer.
 * https://oeis.org/A242801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 1 such that (k^k + n)/(k + n) is an integer
 * Range:     u8 n <= 266, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"


seq_t a(idx_t n)
{
    /* The sequence is defined for all n >= 1; behaviour for n < 1 is
       unspecified.  The existence of such a k is guaranteed for all n.
       The search stops at the first suitable k. */

    if (n < 1) {
        return (seq_t)0;  /* arbitrary value for out-of-range n */
    }

    for (idx_t k = 2; /* infinite loop, guaranteed to terminate */; ++k) {
        seq_t mod = (seq_t)(k + n);            // divisor k + n
        seq_t rem = mod_pow((seq_t)k, k, mod); // k^k mod (k+n)

        /* Condition: (k^k + n) divisible by (k + n) */
        if (((rem + (seq_t)n) % mod) == 0) {
            return (seq_t)k;
        }
    }
}
