/*
 * A346813  Number of partitions of the (n+3)-multiset {0,0,0,1,2,...,n} into
 *          distinct multisets.
 * https://oeis.org/A346813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{t=1}^{n} S(n,t) * (C(t+3,3)+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where S(n,t) is the Stirling number of the second kind. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static state */
    static size_t  cur_n = 0;          /* largest n computed so far   */
    static size_t  a_cap = 0;          /* capacity of a_vals          */
    static seq_t  *a_vals = NULL;      /* table of a(n) values        */
    static size_t  stir_cap = 0;       /* capacity of stir array      */
    static seq_t  *stir = NULL;        /* array holding S(i,t) values */

    /* On first call initialise */
    if (a_cap == 0) {
        a_cap = 1;
        a_vals = (seq_t *)calloc(a_cap, sizeof(seq_t));
        if (!a_vals) exit(1);
        a_vals[0] = (seq_t)2;           /* a(0)=2 */

        stir_cap = 1;
        stir = (seq_t *)calloc(stir_cap, sizeof(seq_t));
        if (!stir) exit(1);
        stir[0] = (seq_t)1;             /* S(0,0)=1 */
        cur_n = 0;
    }

    /* Extend tables if necessary */
    if (n > cur_n) {
        for (size_t i = cur_n + 1; i <= (size_t)n; ++i) {
            /* Grow stir if needed */
            if (i >= stir_cap) {
                size_t new_cap = stir_cap > 0 ? stir_cap * 2 : 1;
                while (new_cap <= i) new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(stir, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                /* initialise new entries */
                for (size_t j = stir_cap; j < new_cap; ++j) tmp[j] = (seq_t)0;
                stir = tmp;
                stir_cap = new_cap;
            }

            /* Update Stirling numbers S(i,t) in place (high → low) */
            for (size_t t = i; t >= 1; --t) {
                stir[t] = stir[t - 1] + (seq_t)t * stir[t];
            }
            stir[0] = (seq_t)0;   /* S(i,0)=0 for i>0 */

            /* Compute a(i) = sum_{t=1}^{i} S(i,t)*g(t)   with g(t)=C(t+3,3)+1 */
            seq_t sum = (seq_t)0;
            for (size_t t = 1; t <= i; ++t) {
                seq_t g = ((seq_t)(t + 3) * (t + 2) * (t + 1)) / (seq_t)6 + (seq_t)1;
                sum += stir[t] * g;
            }

            /* Grow a_vals if needed */
            if (i >= a_cap) {
                size_t new_cap = a_cap > 0 ? a_cap * 2 : 1;
                while (new_cap <= i) new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                for (size_t j = a_cap; j < new_cap; ++j) tmp[j] = (seq_t)0;
                a_vals = tmp;
                a_cap = new_cap;
            }
            a_vals[i] = sum;
        }
        cur_n = (size_t)n;
    }

    return a_vals[n];
}
