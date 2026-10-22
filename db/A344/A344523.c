/*
 * A344523  a(n) = Sum_{1 <= i, j, k, l <= n} gcd(i,j,k,l).
 * https://oeis.org/A344523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} phi(k) * floor(n/k)^4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 249, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* compute Euler's totient values up to n in a temporary uint64_t array */
    uint64_t *phi = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    if (!phi) return 0;  /* allocation error */

    for (idx_t i = 0; i <= n; ++i) phi[i] = (uint64_t)i;
    for (idx_t i = 2; i <= n; ++i) {
        if (phi[i] == (uint64_t)i) {           /* i is prime */
            for (idx_t j = i; j <= n; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = n / k;                 /* floor(n/k) */
        seq_t p  = (seq_t)t;
        seq_t p2 = p * p;
        seq_t p4 = p2 * p2;
        sum += (seq_t)phi[k] * p4;
    }

    free(phi);
    return sum;
}
