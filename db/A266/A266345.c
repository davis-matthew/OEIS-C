/*
 * A266345  Positions of records in A266344.
 * https://oeis.org/A266345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n-th record in A266344, where A266344(m) =
 *            Sum_{d|m, d<=m/d} [len_fact(d)=len_fact(m/d)]; computed by a
 *            doubling sieve over counts of factor pairs with equal
 *            factorial-base length
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A266345_MAXT 4096
static idx_t A266345_pos[A266345_MAXT];
static idx_t A266345_cnt = 0;
static idx_t A266345_limit = 0;

static void A266345_scan(idx_t N)
{
    unsigned char *c = (unsigned char *)calloc((size_t)N + 1, 1);
    if (!c) return;
    idx_t L = 1, fact = 2; /* (L+1)! */
    for (idx_t d = 1; d * d <= N; ++d) {
        while (d >= fact) { /* d > (L+1)!-1 */
            L++;
            fact *= (idx_t)(L + 1);
        }
        idx_t hi = fact - 1;
        idx_t lim = N / d;
        if (lim < hi) hi = lim;
        for (idx_t e = d; e <= hi; ++e)
            c[d * e]++;
    }
    idx_t k = 0;
    int best = 0;
    for (idx_t m = 1; m <= N && k < A266345_MAXT; ++m) {
        if ((int)c[m] > best) {
            best = c[m];
            A266345_pos[k++] = m;
        }
    }
    A266345_cnt = k;
    A266345_limit = N;
    free(c);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A266345_limit == 0) A266345_scan((idx_t)1 << 16);
    while (A266345_cnt < n && A266345_cnt < A266345_MAXT) {
        idx_t before = A266345_limit;
        A266345_scan(A266345_limit * 2);
        if (A266345_limit == before) break;
    }
    return (seq_t)A266345_pos[n - 1];
}
