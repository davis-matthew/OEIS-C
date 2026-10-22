/*
 * A335791  For any n > 1, let x(0) = 2 and for any k >= 0, x(k+1) = (x(k)^2 +
 *          1) mod n, d(k) = gcd(abs(x(k) - x(2*k)), n); necessarily, d(k) > 1
 *          for some k > 0; a(n) is the first such d(k).
 * https://oeis.org/A335791
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first k>0 with gcd(|x(k)-x(2k)|, n)>1, where x(0)=2 and
 *            x(k+1) = (x(k)^2+1) mod n
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                      /* undefined for n <= 1, but not needed */

    /* Allocate array for x[0] .. x[2*n] */
    size_t size = (size_t)(2 * n + 1);
    seq_t *x = (seq_t *)malloc(size * sizeof(seq_t));
    if (!x)
        return 0;                      /* out of memory; caller may handle */

    const seq_t mod = (seq_t)n;
    const seq_t base = (seq_t)2;

    /* Compute the sequence values */
    x[0] = base;
    for (size_t i = 0; i < 2 * n; ++i) {
        /* x[i+1] = (x[i]^2 + 1) mod n */
        /* Use intermediate multiplication safely (overflow only if seq_t is 128-bit or arbitrary); */
        x[i + 1] = (x[i] * x[i] + 1) % mod;
    }

    /* Find the first k>0 with non‑trivial gcd */
    for (size_t k = 1; k <= n; ++k) {
        /* diff = |x[k] - x[2*k]| */
        seq_t diff = (x[k] >= x[2 * k]) ? (x[k] - x[2 * k]) : (x[2 * k] - x[k]);
        seq_t d = gcd(diff, mod);
        if (d > 1) {
            free(x);
            return d;
        }
    }

    /* Should never reach here for n >= 2, but fall back to n */
    free(x);
    return (seq_t)n;
}
