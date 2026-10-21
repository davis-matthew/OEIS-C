/*
 * A244333  Number of ternary digits in the high-water marks of the terms of
 *          the continued fraction of the base 3 Champernowne constant
 *          (A077772).
 * https://oeis.org/A244333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   a(1)=0, a(2)=1, a(3)=1, a(4)=4 For n>=5: a(n) = NCD(n) -
 *            2*NCD(n-1) - 3*(n-2) + 4 where NCD(N)=3-N + Σ_{m=1}^{N-3}
 *            2*m*3^{m-1} Using recurrence: NCD(n) = NCD(n-1) - 1 +
 *            2*(n-3)*3^{n-4}
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *a_val = NULL;      /* a_val[i] = a(i)  (1‑based) */
    static size_t a_cap = 0;
    static seq_t *pow3 = NULL;       /* pow3[k] = 3^k  (k>=0) */
    static size_t pow_cap = 0;
    static idx_t  n_computed = 0;    /* largest n for which entries exist */
    static seq_t  lastNCD = 0;       /* NCD(n_computed) */

    /* initialise first four terms */
    if (a_val == NULL) {
        a_cap = 5;  /* indices 0..4 */
        a_val = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_val[0] = 0;   /* unused (offset 1) */
        a_val[1] = 0;
        a_val[2] = 1;
        a_val[3] = 1;
        a_val[4] = 4;
        n_computed = 4;
        lastNCD = 1;    /* NCD(4) = 1 */

        pow_cap = 1;    /* pow3[0] = 1 */
        pow3 = (seq_t *)malloc(pow_cap * sizeof(seq_t));
        pow3[0] = (seq_t)1;
    }

    if (n <= 0)
        return (seq_t)0; /* undefined, but safe */

    if (n <= n_computed)
        return a_val[n];

    /* expand a_val array if necessary */
    if ((size_t)(n + 1) > a_cap) {
        size_t new_cap = a_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        a_val = (seq_t *)realloc(a_val, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }

    /* expand pow3 array if necessary */
    /* pow3 index needed = n-4 (for n>=5) */
    size_t max_pow_needed = (n >= 4) ? (size_t)(n - 4) : 0;
    while (pow_cap <= max_pow_needed) {
        seq_t next = pow3[pow_cap - 1] * (seq_t)3;
        pow3 = (seq_t *)realloc(pow3, (pow_cap + 1) * sizeof(seq_t));
        pow3[pow_cap] = next;
        pow_cap++;
    }

    /* compute missing a(k) values */
    for (idx_t m = n_computed + 1; m <= n; ++m) {
        if (m < 5) {
            /* This block is never reached because m starts at n_computed+1 >=5 */
            continue;
        }
        /* compute NCD(m) using recurrence */
        size_t pow_index = (size_t)(m - 4);            /* 3^(m-4) */
        seq_t pow_val = pow3[pow_index];
        seq_t mult_factor = ((seq_t)2 * (seq_t)(m - 3));  /* 2*(m-3) */
        seq_t newNCD = lastNCD - (seq_t)1 + mult_factor * pow_val;

        /* compute a(m) */
        seq_t term = newNCD
                     - lastNCD * (seq_t)2
                     - (seq_t)3 * (seq_t)(m - 2)
                     + (seq_t)4;
        a_val[m] = term;

        /* update state for next iteration */
        lastNCD = newNCD;
    }

    n_computed = n;
    return a_val[n];
}
