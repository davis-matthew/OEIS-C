/*
 * A398522  a(n) = Sum_{k=1..n} tau_3(floor(n/k)), where tau_3 = A007425 is
 *          the 3rd Piltz function.
 * https://oeis.org/A398522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} tau_3(floor(n/k))
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _tau3_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the 3rd Piltz function via smallest prime factors. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/_tau3_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1 */

    /* Allocate array of smallest prime factors up to n */
    size_t sz = (size_t)(n + 1);
    idx_t *spf = (idx_t *)calloc(sz, sizeof(idx_t));
    if (!spf) return 0;               /* allocation failure */

    spf[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i * i <= n) {
                for (idx_t j = i * i; j <= n; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }

    /* Precompute tau_3 values up to n */
    seq_t *tau3 = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!tau3) {
        free(spf);
        return 0;
    }
    tau3[0] = 0;
    tau3[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t p = spf[i];
        idx_t m = i, e = 0;
        while (m % p == 0) {
            m /= p;
            e++;
        }
        seq_t term = (seq_t)(e + 2) * (seq_t)(e + 1) / 2;
        tau3[i] = tau3[m] * term;
    }

    /* Sum tau_3(floor(n/k)) over k=1..n */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += tau3[n / k];
    }

    free(spf);
    free(tau3);
    return sum;
}
