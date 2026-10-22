/*
 * A351143  G.f. A(x) satisfies: A(x) = 1 + x^2 * A(x/(1 - 2*x)) / (1 - 2*x).
 * https://oeis.org/A351143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0; for n>=2, a(n) = Σ_{k=0}^{n-2} binom(n-2,k) *
 *            2^k * a(n-2-k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cap = 0;
static idx_t max_computed = 0;

static void ensure_capacity(idx_t n)
{
    if (n + 1 > cap) {
        idx_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_capacity(n);

    if (max_computed == 0) {
        /* initialize the base cases */
        cache[0] = 1;
        if (n >= 1) cache[1] = 0;
        max_computed = (n >= 1) ? 1 : 0;
    }

    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        if (cur == 0) { cache[cur] = 1; continue; }
        if (cur == 1) { cache[cur] = 0; continue; }

        idx_t m = cur - 2;            /* number of terms in the sum            */
        seq_t sum = 0;
        seq_t binom = 1;              /* binom(m,0)                            */
        seq_t t2 = 1;                 /* 2^0                                   */

        for (idx_t k = 0; k <= m; ++k) {
            seq_t term = binom * t2;
            term *= cache[m - k];
            sum += term;

            if (k < m) {
                binom = binom * (seq_t)(m - k) / (seq_t)(k + 1);
                t2 <<= 1;
            }
        }
        cache[cur] = sum;
    }

    max_computed = (max_computed > n) ? max_computed : n;
    return cache[n];
}
