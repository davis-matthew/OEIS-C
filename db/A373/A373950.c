/*
 * A373950  Number of integer compositions of n containing two adjacent ones
 *          and no other runs.
 * https://oeis.org/A373950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i+j=n-2} f(i)*f(j) for n>=2, a(0)=a(1)=0, where c(m)
 *            = number of Carlitz compositions of m (g.f. 1/(1 - Sum_{i>0}
 *            x^i/(1+x^i))), f(0)=1, f(m)=c(m)-f(m-1)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 77, u128 n <= 155,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 900 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    size_t m = (size_t)(n - 1);
    int *s = (int *)malloc((m + 1) * sizeof(int));
    for (size_t k = 1; k <= m; k++) {
        int v = 0;
        for (size_t i = 1; i <= k; i++) {
            if (k % i == 0) {
                size_t q = k / i;
                v += (q & 1) ? 1 : -1;
            }
        }
        s[k] = v;
    }
    seq_t *c = mem_new(seq_t, m + 1);
    seq_t *f = mem_new(seq_t, m + 1);
    c[0] = 1;
    for (size_t k = 1; k <= m; k++) {
        seq_t pos = 0, neg = 0;
        for (size_t j = 1; j <= k; j++) {
            int sj = s[j];
            if (sj > 0) pos += (seq_t)sj * c[k - j];
            else if (sj < 0) neg += (seq_t)(-sj) * c[k - j];
        }
        c[k] = pos - neg;
    }
    f[0] = 1;
    for (size_t k = 1; k <= m; k++) f[k] = c[k] - f[k - 1];
    seq_t res = 0;
    size_t t = (size_t)(n - 2);
    for (size_t i = 0; i <= t; i++) res += f[i] * f[t - i];
    free(s);
    mem_free(c);
    mem_free(f);
    return res;
}
