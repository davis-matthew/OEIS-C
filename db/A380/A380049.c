/*
 * A380049  Partial sums of A072203.
 * https://oeis.org/A380049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A072203(k), with A072203(k) = 1 -
 *            Sum_{j=1..k} (-1)^bigomega(j); computed by a sieve
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    size_t sz = (size_t)n + 1;
    uint32_t *spf = (uint32_t *)calloc(sz, sizeof(uint32_t));
    unsigned char *par = (unsigned char *)calloc(sz, 1);
    if (!spf || !par) {
        free(spf);
        free(par);
        return 0;
    }

    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i <= n / i) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }

    long long L = 1;          /* running Liouville sum, lambda(1) = 1 */
    seq_t total = 0;          /* sum of (1 - L(k)) */
    total += (seq_t)0;        /* k = 1: 1 - 1 = 0 */
    for (idx_t i = 2; i <= n; ++i) {
        par[i] = par[i / spf[i]] ^ 1;
        L += par[i] ? -1 : 1;
        total += (seq_t)(1 - L);
    }

    free(spf);
    free(par);
    return total;
}
