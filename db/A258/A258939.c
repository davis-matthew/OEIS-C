/*
 * A258939  Expansion of f(-x^3, -x^5) * f(x^3, x^13) / (f(-x, -x^2) * f(-x^8,
 *          -x^16)) in powers of x where f(, ) is Ramanujan's general theta
 *          function.
 * https://oeis.org/A258939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the Euler transform of the period‑32 sequence
 *            [1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,0].
 *            Explicitly, a(0)=1 and for n≥1 a(n) = (1/n) * Σ_{k=1}^{n} (
 *            Σ_{d|k} d·a(d) ) · a(n−k)
 * Range:     u8 n <= 9, u16 n <= 30, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------- */
#include <stdlib.h>
#include <stdbool.h>

/* period‑32 binary sequence, indexes 0..31 */
static const seq_t PERIOD32[32] = {
    (seq_t)1,(seq_t)1,(seq_t)1,(seq_t)1,
    (seq_t)0,(seq_t)0,(seq_t)1,(seq_t)1,
    (seq_t)1,(seq_t)1,(seq_t)0,(seq_t)1,
    (seq_t)1,(seq_t)1,(seq_t)1,(seq_t)0,
    (seq_t)1,(seq_t)1,(seq_t)1,(seq_t)1,
    (seq_t)0,(seq_t)1,(seq_t)1,(seq_t)1,
    (seq_t)1,(seq_t)0,(seq_t)0,(seq_t)1,
    (seq_t)1,(seq_t)1,(seq_t)1,(seq_t)0
};

/* a(d) for d≥1: look‑up in the period‑32 sequence */
static inline seq_t a_of(idx_t d)
{
    return PERIOD32[(d - 1) & 31];
}

/* ensure that *arr has at least sz elements of type seq_t */
static void ensure_size(seq_t **arr, size_t *size, size_t sz)
{
    if (*size >= sz) return;
    size_t newsize = (*size == 0) ? sz : (*size * 2);
    if (newsize < sz) newsize = sz;
    seq_t *tmp = (seq_t *)realloc(*arr, newsize * sizeof(seq_t));
    if (!tmp) exit(1);             /* allocation failure */
    *arr   = tmp;
    *size  = newsize;
}

/* -------------------------------------------------------------- */
/*  Main function:  seq_t a(idx_t n)  */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *b = NULL;          /* Euler transform values (0..cur_max) */
    static seq_t *c = NULL;          /* divisor‑sum values (0..cur_max) */
    static size_t b_size = 0;
    static size_t c_size = 0;
    static idx_t cur_max = 0;        /* largest n computed so far */

    /* base case a(0) = 1 */
    if (n == 0) {
        if (cur_max == 0) {
            ensure_size(&b, &b_size, 1);
            b[0] = (seq_t)1;
            cur_max = 0;
        }
        return b[0];
    }

    /* extend memoised data up to n */
    if (n > cur_max) {
        /* allocate space for new indices */
        ensure_size(&b, &b_size, n + 1);  /* index 0..n inclusive */
        ensure_size(&c, &c_size, n + 1);  /* index 0..n inclusive */

        /* compute c(k) = Σ_{d|k} d·a(d) for k = cur_max+1 .. n */
        for (idx_t k = cur_max + 1; k <= n; ++k) {
            seq_t sum = 0;
            for (idx_t d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    /* divisor d */
                    seq_t term = (seq_t)d * a_of(d);
                    sum += term;
                    /* paired divisor k/d, avoid double‑counting square root */
                    idx_t e = k / d;
                    if (e != d) {
                        term = (seq_t)e * a_of(e);
                        sum += term;
                    }
                }
            }
            c[k] = sum;
        }

        /* compute b(m) for m = cur_max+1 .. n using the Euler transform */
        for (idx_t m = cur_max + 1; m <= n; ++m) {
            seq_t inner = 0;
            for (idx_t k = 1; k <= m; ++k) {
                inner += c[k] * b[m - k];
            }
            b[m] = inner / (seq_t)m;   /* exact division by definition */
        }

        cur_max = n;
    }

    return b[n];
}
