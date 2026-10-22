/*
 * A386595  a(n) = Sum_{d|n} sigma(d)/phi(d) * c(d), where c = A351114.
 * https://oeis.org/A386595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [phi(d) | sigma(d)] * sigma(d)/phi(d)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (c(d) is 1 iff phi(d) divides sigma(d), otherwise 0)  */
#include <stdlib.h>
#include <string.h>

static idx_t currentMax = 0;
#ifdef __cplusplus
    static seq_t *sigma = nullptr;
    static seq_t *phi   = nullptr;
#else
    static seq_t *sigma = NULL;
    static seq_t *phi   = NULL;
#endif

static void recompute_arrays(idx_t n)
{
    /* free old arrays */
    if (sigma) free(sigma);
    if (phi)   free(phi);

    /* allocate new arrays, zero sigma */
    sigma = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    phi   = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!sigma || !phi) exit(EXIT_FAILURE);
    memset(sigma, 0, (n + 1) * sizeof(seq_t));

    /* compute sigma (sum of divisors) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        for (idx_t j = i; j <= n; j += i) {
            sigma[j] += ii;
        }
    }

    /* compute phi (Euler's totient) */
    for (idx_t i = 1; i <= n; ++i) phi[i] = (seq_t)i;
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == (seq_t)p) {            /* p is prime */
            for (idx_t k = p; k <= n; k += p) {
                phi[k] -= phi[k] / (seq_t)p;
            }
        }
    }

    currentMax = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0 */
    if (n > currentMax) recompute_arrays(n);

    seq_t result = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor d = i */
            seq_t s = sigma[i];
            seq_t t = phi[i];
            if (t != 0 && s % t == 0) result += s / t;

            /* corresponding divisor e = n/i, avoid double counting */
            idx_t e = n / i;
            if (e != i) {
                s = sigma[e];
                t = phi[e];
                if (t != 0 && s % t == 0) result += s / t;
            }
        }
    }
    return result;
}
