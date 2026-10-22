/*
 * A398528  a(n) = Sum_{k=1..n} omega(floor(n/k)), where omega(n) = A001221(n)
 *          is the number of distinct primes dividing n.
 * https://oeis.org/A398528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} omega(floor(n/k))
 * Range:     u8 n <= 419, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* safeguard, offset starts at 1 */

    /* Allocate smallest prime factor array (spf) for 1..n */
    idx_t* spf = (idx_t*)calloc((size_t)n + 1, sizeof(idx_t));
    if (!spf) return (seq_t)0;       /* allocation failure */

    /* Sieve of Eratosthenes to fill spf */
    for (idx_t i = 2; i <= n; ++i) spf[i] = i;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= n; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }

    /* Compute the omega (distinct prime factor count) array */
    seq_t* omega = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!omega) { free(spf); return (seq_t)0; }
    omega[0] = omega[1] = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t p = spf[i];
        idx_t m = i / p;
        if (p == spf[m]) {
            omega[i] = omega[m];
        } else {
            omega[i] = omega[m] + (seq_t)1;
        }
    }

    /* Sum omega(floor(n/k)) over k = 1..n */
    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = n / k;
        result += omega[t];
    }

    free(spf);
    free(omega);
    return result;
}
