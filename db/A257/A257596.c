/*
 * A257596  G.f.: (1-x)*(1-2*x-x^2-(1-x)*sqrt(1-2*x-3*x^2))/(2*x*(1-2*x-x^2)).
 * https://oeis.org/A257596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257596
 * Recurrence: (n+1)*a(n)+(-5*n+1)*a(n-1)+4*(n-2)*a(n-2)+8*(n-2)*a(n-3)+(-5*n+31)*a(n-4)+3*(-n+5)*a(n-5)=0
 * Initial values (offset 1): a(1)=0,a(2)=0,a(3)=1,a(4)=3,a(5)=10
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* offset 1 – n = 0 is unused */

    /* Static memoization cache, 1‑based indexing.
       cache[0] is a dummy slot.  */
    static seq_t *cache = NULL;
    static idx_t last_computed = 0;   /* largest index computed so far */

    /* Initialise cache with the first 5 terms.  */
    if (cache == NULL) {
        cache = (seq_t*)malloc(6 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failed */
        cache[0] = 0;              /* unused */
        cache[1] = 0;              /* a(1) */
        cache[2] = 0;              /* a(2) */
        cache[3] = 1;              /* a(3) */
        cache[4] = 3;              /* a(4) */
        cache[5] = 10;             /* a(5) */
        last_computed = 5;
    }

    /* Grow the cache if we need to compute larger indices.  */
    if (n > last_computed) {
        size_t new_size = (size_t)(n + 1); /* index 0..n inclusive */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failed */
        cache = tmp;

        /* Compute all terms from last_computed+1 up to n.  */
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* Positive‑coefficient terms.  */
            seq_t sum_pos = 0;
            sum_pos += cache[i-2] * (seq_t)(4 * (i-2));   /* 4*(n-2)*a(n-2) */
            sum_pos += cache[i-3] * (seq_t)(8 * (i-2));   /* 8*(n-2)*a(n-3) */

            /* Negative‑coefficient terms – accumulate absolute values.  */
            seq_t sum_neg = 0;
            sum_neg += cache[i-1] * (seq_t)(5 * i - 1);    /* |-5*n+1| * a(n-1) */

            /* Handle a(n-4) with coefficient (-5*n+31).  */
            int coeff_a4 = -5 * (int)i + 31;
            if (coeff_a4 >= 0) {
                sum_pos += cache[i-4] * (seq_t)(coeff_a4);
            } else {
                sum_neg += cache[i-4] * (seq_t)(-coeff_a4);
            }

            /* Handle a(n-5) with coefficient 3*(-n+5) = 15-3*n.  */
            int coeff_a5 = 15 - 3 * (int)i;
            if (coeff_a5 >= 0) {
                sum_pos += cache[i-5] * (seq_t)(coeff_a5);
            } else {
                sum_neg += cache[i-5] * (seq_t)(-coeff_a5);
            }

            /* The recurrence gives a(n) = (sum_neg – sum_pos)/(n+1).  */
            seq_t diff = sum_neg - sum_pos;          /* guaranteed non‑negative */
            cache[i] = diff / (seq_t)(i + 1);
        }

        last_computed = n;
    }

    return cache[n];
}
