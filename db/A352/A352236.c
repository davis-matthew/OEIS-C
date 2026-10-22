/*
 * A352236  G.f. A(x) satisfies: A(x) = 1 + x*A(x)^2 / (A(x) - 2*x*A'(x)).
 * https://oeis.org/A352236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1; For n ≥ 2: a(n) = 2 ∑_{k=1}^{n-1} k a(k) a(n–k)
 *            + 2 a(n–1) – (∑_{k=1}^{n-1} a(k) a(n–k) – ∑_{k=1}^{n-2} a(k)
 *            a(n–1–k))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *tab = NULL;
    static size_t cap  = 0;            /* number of elements allocated   */
    static idx_t last = 0;             /* largest index computed yet   */
    static seq_t p_prev = 0;           /* p_{last-1} = ∑_{k=1}^{last-1} a(k) a(last–k)   */

    /* ensure table is large enough for indices 0 … n */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap : 1;
        while ((size_t)n >= newcap) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(tab, newcap * sizeof(seq_t));
        if (!tmp) exit(1);            /* abort on memory failure      */
        tab  = tmp;
        cap  = newcap;
    }

    /* initialise a(0) and a(1) if not done yet */
    if (last == 0)
    {
        tab[0] = (seq_t)1;
        last   = 0;
        p_prev = (seq_t)0;   /* p_0 = 0 */
    }
    if (n == 0) return tab[0];
    if (n == 1) {
        if (last < 1) {
            tab[1] = (seq_t)1;
            last   = 1;
            p_prev = (seq_t)0;   /* p_1 = 0 */
        }
        return tab[1];
    }

    /* compute values up to n incrementally */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;   /* ∑ k * a(k) * a(i–k)          */
        seq_t p   = (seq_t)0;   /* ∑ a(k) * a(i–k)              */

        for (idx_t k = 1; k < i; ++k)
        {
            seq_t prod = tab[k] * tab[i - k];
            sum += prod * (seq_t)k;
            p   += prod;
        }

        /* a(i) = 2*sum + 2*a(i-1) – (p – p_prev) */
        seq_t double_sum   = sum + sum;
        seq_t double_prev  = tab[i - 1] + tab[i - 1];
        seq_t diff_p       = p - p_prev;
        tab[i] = double_sum + double_prev - diff_p;

        /* update for next iteration */
        p_prev = p;
        last   = i;
    }

    return tab[n];
}
