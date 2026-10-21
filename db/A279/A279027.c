/*
 * A279027  a(n) = number of primes of the form 4k + 3 such that 2n - 4k - 3
 *          is prime.
 * https://oeis.org/A279027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { primes p ≡ 3 (mod 4) | 2n – p is also prime }
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate over primes of the form 4k+3 up to 2n-3 and count
   those with complementary prime 2n – p. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; n=0 never used */

    idx_t two_n = n + n;                 /* 2 * n */
    idx_t limit = (two_n < 3) ? 0 : two_n - 3;   /* highest possible p */

    seq_t cnt = 0;

    /* Iterate over primes of the form 4k+3: start at 3, step by 4. */
    for (idx_t p = 3; p <= limit; p += 4) {
        if (!is_prime(p))
            continue;
        idx_t q = two_n - p;            /* 2n – p */
        if (q >= 2 && is_prime(q))
            cnt++;
    }

    return cnt;
}
