/*
 * A323079  Lexicographically earliest such sequence a that a(i) = a(j) =>
 *          f(i) = f(j) for all i, j, where f(n) = -1 if n is an odd prime,
 *          and f(n) = A060681(n) for all other numbers.
 * https://oeis.org/A323079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of first occurrence of f(n) among f(1..n), where
 *            f(n) = -1 if n is an odd prime, else A060681(n) = n - n/spf(n)
 *            (f(1)=0); computed by a sieve of smallest prime factors and a
 *            scan
 * Range:     u8 n <= 421, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 120 terms through n = 9690 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t *spf = (idx_t *)calloc((size_t)n + 1, sizeof(idx_t));
    idx_t *id  = (idx_t *)calloc((size_t)n + 3, sizeof(idx_t));
    if (!spf || !id) { free(spf); free(id); return 0; }

    for (idx_t i = 2; i <= n; i++) {
        if (spf[i] == 0) {
            for (idx_t j = i; j <= n; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }

    idx_t count = 0;
    idx_t result = 0;
    for (idx_t m = 1; m <= n; m++) {
        idx_t key;
        if (m == 1) key = 1;                       /* f = 0 */
        else if (spf[m] == m && m > 2) key = 0;    /* f = -1 */
        else key = (m - m / spf[m]) + 1;
        if (id[key] == 0) id[key] = ++count;
        if (m == n) result = id[key];
    }

    free(spf);
    free(id);
    return (seq_t)result;
}
