/*
 * A394472  a(n) = Product_{k=1..n} (2*prime(n)-prime(k)).
 * https://oeis.org/A394472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{k=1..n} (2*prime(n)-prime(k))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0) is undefined */

    uint32_t pn = get_prime(n);           /* prime(n) */
    seq_t two_p = (seq_t)2 * (seq_t)pn;   /* 2*prime(n) */

    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        uint32_t pk = get_prime(k);       /* prime(k) */
        prod *= (two_p - (seq_t)pk);
    }
    return prod;
}
