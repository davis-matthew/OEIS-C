/*
 * A317411  Expansion of e.g.f. tanh(x/(1 - x)).
 * https://oeis.org/A317411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1;
   For n≥1: a(n+1) = 2*n*a(n) - n*(n-1)*a(n-1)
              - Σ_{k=0}^n binom(n,k)*a(k)*a(n-k) */
/* The recurrence follows from the differential equation
   (1–x)² G'(x) = 1 – G(x)² where G(x) = tanh(x/(1–x)). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;   /* cache of computed values      */
    static idx_t last = 0;     /* highest index already stored */

    if (n == 0)
        return 0;              /* a(0) is the first term       */

    /* initialise cache and first terms */
    if (buf == NULL) {
        buf = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!buf) return 0;          /* allocation failure (unlikely) */
        buf[0] = 0;
        buf[1] = 1;
        last  = 1;
    }

    /* enlarge buffer if necessary (doubling strategy) */
    if (n > last) {
        idx_t cap = last + 1;
        while (cap <= n)
            cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(buf, cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure (unlikely) */
        buf = tmp;
        /* 'last' remains the highest index computed so far */
    }

    /* compute missing values up to n */
    for (idx_t cur = last + 1; cur <= n; ++cur) {
        idx_t k = cur - 1;           /* k = n in the recurrence */
        seq_t sum = 0;               /* convolution part */
        seq_t binom = 1;             /* C(k,0)          */
        idx_t half = k >> 1;         /* floor(k/2)       */

        for (idx_t i = 0; i <= half; ++i) {
            seq_t prod = buf[i] * buf[k - i];
            seq_t term = prod * binom;
            if (i < k - i)            /* pair with its symmetric counterpart */
                sum += term * 2;
            else
                sum += term;          /* middle term when k even */

            if (i < half)            /* advance binomial coefficient */
                binom = binom * (seq_t)(k - i) / (seq_t)(i + 1);
        }

        seq_t tmp_val = buf[k] * (seq_t)(2 * k);        /* 2*n*a(n)          */
        tmp_val -= buf[k - 1] * (seq_t)(k * (k - 1));   /* n*(n-1)*a(n-1)    */
        tmp_val -= sum;                               /* convolution        */
        buf[cur] = tmp_val;
    }

    last = n;
    return buf[n];
}
