/*
 * A344521  a(n) = Sum_{1 <= i <= j <= k <= n} gcd(i,j,k).
 * https://oeis.org/A344521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} phi(k) * binom(floor(n/k)+2, 3)
 * Range:     u8 n <= 9, u16 n <= 64, u32 n <= 2658, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where phi is Euler's totient and binom(m,3)=m*(m-1)*(m-2)/6   */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 */

    /* 1. Compute totients phi[1..n] with a simple sieve             */
    uint32_t *phi = (uint32_t *)calloc((size_t)n + 1, sizeof(uint32_t));
    if (!phi) return (seq_t)0;            /* allocation failure */

    for (idx_t i = 0; i <= n; ++i) phi[i] = (uint32_t)i;

    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == (uint32_t)p) {      /* p is prime */
            for (idx_t k = p; k <= n; k += p) {
                phi[k] -= phi[k] / (uint32_t)p;
            }
        }
    }

    /* 2. Accumulate the sum                                        */
    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = n / k;                  /* floor(n/k)                     */
        /* binom(m+2,3) = (m+2)*(m+1)*m / 6, computed in 64‑bit */
        unsigned long long binom =
            (unsigned long long)(m + 2) *
            (unsigned long long)(m + 1) *
            (unsigned long long)m / 6ULL;
        seq_t term = (seq_t)phi[k] * (seq_t)binom;
        sum += term;
    }

    free(phi);
    return sum;
}
