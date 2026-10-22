/*
 * A307816  a(1) = 1; a(n) = Sum_{k=1..n-1} a(n-k) * Sum_{d|k} a(d)*a(k/d).
 * https://oeis.org/A307816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1 a(n) = ∑_{k=1}^{n-1} a(n-k) * ∑_{d|k} a(d) *
 *            a(k/d) (OEIS A307816)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence and the divisor‐sum intermediate
   but only after the corresponding index has been computed. */
static seq_t *a_val   = NULL; /* a[1 ..]                      */
static seq_t *S_val   = NULL; /* S(k) = ∑_{d|k} a(d)*a(k/d)    */
static size_t  cap     = 0;    /* current allocated size         */
static size_t  cur_max = 0;    /* highest index that has been filled */

seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined, but harmless          */
        return (seq_t)0;

    /* Grow the cached arrays to accommodate index n.  The new
       capacity is chosen as a power of two ≥ n+1.            */
    if (cap < (size_t)n + 1) {
        size_t new_cap = cap ? cap << 1 : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp;
        tmp = (seq_t *)realloc(a_val, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_val = tmp;

        tmp = (seq_t *)realloc(S_val, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        S_val = tmp;

        /* initialise any new elements to zero */
        for (size_t i = cap; i < new_cap; ++i) {
            a_val[i] = (seq_t)0;
            S_val[i] = (seq_t)0;
        }
        cap = new_cap;
    }

    /* Compute all terms up to n that have not yet been done. */
    for (size_t k = cur_max + 1; k <= (size_t)n; ++k) {
        if (k == 1) {                   /* base case */
            a_val[1] = (seq_t)1;
            S_val[1] = (seq_t)1;        /* ∑_{d|1} a(d)*a(1/d) = a(1)*a(1) = 1 */
            continue;
        }

        /* a(k) = ∑_{j=1}^{k-1} a(k-j) * S(j) */
        seq_t sum = (seq_t)0;
        for (size_t j = 1; j < k; ++j)
            sum += a_val[k - j] * S_val[j];
        a_val[k] = sum;

        /* S(k) = ∑_{d|k} a(d)*a(k/d)  (count both d and k/d) */
        seq_t Sk = (seq_t)0;
        for (size_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                size_t e = k / d;                  /* co‑divisor */
                Sk += a_val[d] * a_val[e];
                if (e != d)                       /* add the symmetric pair */
                    Sk += a_val[d] * a_val[e];
            }
        }
        S_val[k] = Sk;
    }

    cur_max = (size_t)n;
    return a_val[n];
}
