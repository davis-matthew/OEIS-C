/*
 * A341117  a(n) = Sum_{i+j>=m+1} d_i * d_j, where d_1 < ... < d_m are the
 *          divisors of n.
 * https://oeis.org/A341117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{i+j≥m+1} d_i · d_j (d_1<…<d_m are the divisors of n)
 * Range:     u8 n <= 9, u16 n <= 107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Comparator for qsort, compares seq_t values */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    /* find all divisors of n */
    /* maximum number of divisors for n≤10000 is far below 512 */
    enum { MAX_DIV = 512 };
    seq_t divs[MAX_DIV];
    size_t m = 0;

    for (idx_t d = 1; d * (idx_t)d <= n; ++d) {
        if (n % d == 0) {
            divs[m++] = (seq_t)d;
            if (d != n / d) {
                if (m < MAX_DIV)
                    divs[m++] = (seq_t)(n / d);
            }
        }
    }

    /* sort the divisors */
    qsort(divs, m, sizeof(seq_t), cmp_seq);

    /* prefix sums of divisors */
    seq_t pref[MAX_DIV + 1];
    pref[0] = 0;
    for (size_t i = 0; i < m; ++i)
        pref[i + 1] = pref[i] + divs[i];

    seq_t sigma = pref[m];  /* sum of all divisors */

    /* compute S = ∑_{i=1}^{m} ∑_{j=1}^{m-i} d_i · d_j
       indices are 0‑based here */
    seq_t S = 0;
    for (size_t i = 0; i < m; ++i) {
        if (m <= 1) break; /* no pairs exist */
        /* maximum j satisfying i + j < m (since i+1+j+1 ≤ m) */
        size_t maxj = (m >= 2) ? (m - 2 - i) : 0;
        if ((int)maxj < 0) continue;
        seq_t sumj = pref[maxj + 1];
        S += divs[i] * sumj;
    }

    /* final value: (sum d)^2 - S */
    return sigma * sigma - S;
}
