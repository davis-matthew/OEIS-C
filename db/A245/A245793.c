/*
 * A245793  Number of preferential arrangements of n labeled elements when at
 *          least k=8 elements per rank are required.
 * https://oeis.org/A245793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1..7) = 0 For n ≥ 1 a(n) = ∑_{i=0}^{n‑1} C(n,i)·a(i)
 *            – n·a(n‑1) – ∑_{i=2}^{min(7,n)} C(n,i)·a(n‑i) where C(n,i) is
 *            the binomial coefficient. This recurrence follows directly from
 *            the e.g.f. 1/(2 + x – eˣ + x²/2! + … + x⁷/7!)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 23, u64 n <= 34, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for the sequence values */
    static seq_t *buf    = NULL;
    static size_t cap    = 0;   /* allocated capacity */
    static size_t last   = 0;   /* highest index computed (inclusive) */

    /* Allocate and initialise the first value */
    if (!buf) {
        cap  = (n < 8 ? 8 : 1);
        buf  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf) return (seq_t)0;   /* out‑of‑memory guard */
        buf[0] = (seq_t)1;          /* a(0) = 1 */
        last   = 0;
    }

    /* Grow the buffer if necessary */
    if (n >= cap) {
        size_t newcap = (cap == 0) ? n + 1 : cap * 2;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        buf = tmp;
        cap = newcap;
    }

    /* Ensure the first eight values are defined */
    if (last < 7) {
        for (idx_t i = last + 1; i <= 7 && i <= n; ++i)
            buf[(size_t)i] = (i == 0 ? (seq_t)1 : (seq_t)0);
        last = 7;
    }

    /* Return directly if the requested index is already computed */
    if (n <= last) return buf[(size_t)n];

    /* Compute new values up to n */
    for (idx_t m = last + 1; m <= n; ++m) {
        seq_t val;
        if (m <= 7) {
            /* The first eight terms are known */
            val = (m == 0 ? (seq_t)1 : (seq_t)0);
        } else {
            /* sum1 = Σ_{i=0}^{m-1} C(m,i) * a(i) */
            seq_t sum1 = (seq_t)0;
            seq_t C    = (seq_t)1;          /* C(m,0) */
            for (idx_t i = 0; i < m; ++i) {
                sum1 += C * buf[(size_t)i];
                seq_t num = (seq_t)(m - i);
                seq_t den = (seq_t)(i + 1);
                C = C * num / den;
            }

            /* sum2 = Σ_{i=2}^{min(7,m)} C(m,i) * a(m-i) */
            seq_t sum2 = (seq_t)0;
            seq_t C2   = (seq_t)1;
            idx_t lim = (m < 7 ? m : 7);
            for (idx_t i = 1; i <= lim; ++i) {
                C2 = C2 * (seq_t)(m - i + 1) / (seq_t)i;
                if (i >= 2)
                    sum2 += C2 * buf[(size_t)(m - i)];
            }

            /* The recurrence */
            val = sum1 - (seq_t)m * buf[(size_t)(m - 1)] - sum2;
        }
        buf[(size_t)m] = val;
    }
    last = (size_t)n;
    return buf[(size_t)n];
}
