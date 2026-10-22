/*
 * A347859  a(n) = n/2 if n/2 is a prime, else 3(n-1)/2 if (n-1)/2 is prime,
 *          else n.
 * https://oeis.org/A347859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/2 if n/2 is a prime, else 3*(n-1)/2 if (n-1)/2 is a
 *            prime, else n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if ((n & 1ULL) == 0) {                 /* n is even */
        idx_t half = n >> 1;               /* n/2 */
        if (is_prime(half))
            return (seq_t)half;            /* prime-division case */
    } else {                               /* n is odd */
        idx_t half = (n - 1) >> 1;         /* (n-1)/2 */
        if (is_prime(half))
            return (seq_t)3 * (seq_t)half; /* secondary ray */
    }
    return (seq_t)n;                       /* main ray */
}
