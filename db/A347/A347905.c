/*
 * A347905  Array read by antidiagonals, m, n >= 1: T(m,n) is the position of
 *          the first prime (after the two initial terms) in the
 *          Fibonacci-like sequence with initial terms m and n, or 0 if no
 *          such prime exists.
 * https://oeis.org/A347905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,n) where T(m,n) is defined recursively: * T(m,n) = 0
 *            if gcd(m,n) > 1; * T(m,n) = 2 if m + n is prime; * T(m,n) = T(n,
 *            m+n) + 1 otherwise. * The pair (m,n) is obtained by reading the
 *            infinite matrix by * antidiagonals. The function handles any n
 *            >= 1 and memoizes * results internally to...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* determine the antidiagonal coordinates (m, k) for index n */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n) {
        d++;                      /* find smallest d with d(d+1)/2 >= n */
    }
    idx_t prev = (d - 1) * d / 2;  /* number of terms before diagonal d */
    idx_t pos  = n - prev;        /* position inside the diagonal (1‑based) */
    seq_t m = (seq_t)pos;
    seq_t k = (seq_t)(d + 1 - pos);  /* second coordinate */

    /* compute T(m,k) */
    if (gcd(m, k) != 1) {
        return (seq_t)0;
    }

    seq_t a0 = m;
    seq_t a1 = k;
    seq_t cnt = 2;                /* position of a0 + a1 in the sequence */
    seq_t sum = a0 + a1;

    if (is_prime_big(sum)) {
        return cnt;
    }

    /* iterate until a prime is found or a safe limit is reached */
    const seq_t LIMIT = 100000;
    while (cnt < LIMIT) {
        a0 = a1;
        a1 = sum;
        cnt++;
        sum = a0 + a1;
        if (is_prime_big(sum)) {
            return cnt;
        }
    }

    /* no prime found within the safe limit */
    return (seq_t)0;
}
