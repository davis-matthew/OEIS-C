/*
 * A368295  Expansion of e.g.f. exp(-x) / (1 - x*exp(3*x)).
 * https://oeis.org/A368295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} (3*(n-k)-1)^k / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived recurrence: a(n) = n * Σ_{j=0}^{n-1} C(n-1,j) * 3^{n-1-j} * a(j) + (-1)^n */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static tables for already computed values and powers of 3 */
    static seq_t *val   = NULL;    /* a[0..len-1] */
    static size_t  val_cap = 0;
    static idx_t   val_len = 0;

    static seq_t *p3    = NULL;    /* 3^e for e=0..p_len-1 */
    static size_t  p_cap  = 0;
    static idx_t   p_len  = 0;

    /* Initialise storage if needed */
    if (!val) {
        val_cap = 1;
        val = (seq_t *)malloc(val_cap * sizeof(seq_t));
        val[0] = (seq_t)1;               /* a(0) = 1 */
        val_len = 1;
    }

    /* Ensure capacity for a[n] */
    if (n >= val_len) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > val_cap) {
            size_t alloc = new_cap;
            if (alloc < 2 * val_cap) alloc = 2 * val_cap;
            val = (seq_t *)realloc(val, alloc * sizeof(seq_t));
            val_cap = alloc;
        }
        /* Allocate powers of 3 up to exponent n-1 */
        if (!p3) {
            p_cap = 1;
            p3 = (seq_t *)malloc(p_cap * sizeof(seq_t));
            p3[0] = (seq_t)1;            /* 3^0 */
            p_len = 1;
        }
        if (n > 0 && p_len <= n-1) {
            size_t need = (size_t)n;    /* need indices 0..n-1 */
            if (need > p_cap) {
                size_t alloc = need;
                if (alloc < 2 * p_cap) alloc = 2 * p_cap;
                p3 = (seq_t *)realloc(p3, alloc * sizeof(seq_t));
                p_cap = alloc;
            }
            for (idx_t i = p_len; i < need; ++i)
                p3[i] = p3[i-1] * (seq_t)3;
            p_len = need;
        }

        /* Compute values from val_len to n inclusive */
        for (idx_t i = val_len; i <= n; ++i) {
            /* Skip a(0) which is already set */
            if (i == 0) continue;

            seq_t sum = (seq_t)0;
            seq_t c   = (seq_t)1;   /* C(i-1,0) */

            /* Sum over j = 0 .. i-1 : C(i-1,j) * 3^{i-1-j} * a[j] */
            for (idx_t j = 0; j < i; ++j) {
                idx_t e = i-1 - j;                     /* exponent for 3 */
                seq_t tmp  = c * val[j];               /* C * a[j] */
                seq_t term = tmp * p3[e];              /* term = C * a[j] * 3^e */
                sum += term;

                /* Update binomial coefficient for next j */
                if (j + 1 < i) {
                    seq_t num = (seq_t)(i-1 - j);      /* (i-1 - j) */
                    seq_t den = (seq_t)(j+1);          /* (j+1) */
                    c = (c * num) / den;              /* exact */
                }
            }

            /* a(i) = i * sum + (-1)^i */
            seq_t product = sum * (seq_t)i;
            if (i & 1)
                val[i] = product - (seq_t)1;   /* odd n → subtract 1 */
            else
                val[i] = product + (seq_t)1;   /* even n → add 1 */
        }
        val_len = n + 1;
    }

    return val[n];
}
