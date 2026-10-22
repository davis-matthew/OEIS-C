/*
 * A393198  a(n) is the least value of m such that n has a prime number of
 *          ones in its base m expansion.
 * https://oeis.org/A393198
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 302, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) = least m such that n has a prime number of
   ones in its base‑m expansion.  For n ≥ 3 it searches m = 2,…,n−1,
   counting the digits equal to 1 and using the provided is_prime_small
   utility to test primality.  If no such m is found, it falls back to
   m = n−1, which always yields two ones. */

#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 3.  For smaller n we simply
       return n−1 to keep the function well defined. */
    if (n < 3) return (seq_t)(n - 1);

    for (idx_t m = 2; m <= n - 1; ++m) {
        idx_t tmp = n;
        int cnt = 0;
        while (tmp) {
            if (tmp % m == (idx_t)1) ++cnt;
            tmp /= m;
        }
        if (cnt > 0 && is_prime_small((seq_t)cnt))
            return (seq_t)m;
    }

    /* Fallback: n in base (n−1) is "11", which has two ones; 2 is prime. */
    return (seq_t)(n - 1);
}
