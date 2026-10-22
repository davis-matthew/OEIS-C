/*
 * A318789  For n >= 3, a(n) is equal to n-1 plus the alternating sum of all
 *          consecutive prime gaps between odd primes <= n.
 * https://oeis.org/A318789
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + Σ (-1)^{i+1} * (p_{i+1} - p_i) where p_i are the
 *            odd primes ≤ n (p_1 = 3). This is equivalent to the OEIS
 *            definition “a(n) = n-1 plus the alternating sum of all
 *            consecutive prime gaps between odd primes ≤ n”. The function
 *            computes this directly for any n ≥ 3
 * Range:     u8 n <= 288, u16 n <= 9954, u32 n <= 9954, u64 n <= 9954,
 *            u128 n <= 9954, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* External function required by the specification. */
seq_t a(idx_t n)
{
    if (n < 3)                 /* undefined below the offset */
        return (seq_t)0;

    seq_t alt_sum = 0;         /* alternating sum of gaps */
    idx_t  prev    = 3;        /* first odd prime */
    int    sign    = -1;       /* first gap gets a negative sign */

    /* iterate over odd numbers starting at 5 */
    for (idx_t p = 5; p <= n; p += 2) {
        if (is_prime(p)) {
            idx_t gap = p - prev;                /* gap between odd primes */
            /* apply alternating sign */
            if (sign < 0)
                alt_sum -= (seq_t)gap;
            else
                alt_sum += (seq_t)gap;
            sign = -sign;                        /* flip sign for next gap */
            prev = p;                            /* update previous prime */
        }
    }

    /* final value: n-1 + alternating sum of gaps */
    return (seq_t)n - 1 + alt_sum;
}
