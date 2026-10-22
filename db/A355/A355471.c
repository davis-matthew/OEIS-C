/*
 * A355471  Expansion of Sum_{k>=0} (x/(1 - k^2 * x))^k.
 * https://oeis.org/A355471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^(2*(n-k)) * binomial(n-1,k-1), a(0)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *a_vals = NULL;      /* array of a(n) */
    static seq_t *b_vals = NULL;      /* array of current b_k values */
    static size_t capacity = 0;       /* current allocation size of a_vals and b_vals */
    static size_t max_computed = 0;   /* largest n that has been computed so far */
    static seq_t current_sum = 0;     /* a(max_computed) */

    /* initialisation: a(0) = 1 */
    if (a_vals == NULL) {
        capacity = 1;
        a_vals = (seq_t *)malloc(sizeof(seq_t) * capacity);
        a_vals[0] = (seq_t)1;
        current_sum = a_vals[0];
    }

    /* return cached value if we already have it */
    if (n <= max_computed)
        return a_vals[n];

    /* extend cache up to n */
    for (size_t new_n = max_computed + 1; new_n <= (size_t)n; ++new_n) {

        /* make sure we have enough storage */
        if (new_n + 1 > capacity) {
            size_t new_cap = new_n + 1;
            a_vals = (seq_t *)realloc(a_vals, sizeof(seq_t) * new_cap);
            b_vals = (seq_t *)realloc(b_vals, sizeof(seq_t) * new_cap);
            capacity = new_cap;
        }

        if (new_n == 1) {
            /* initialise b[1] = 1 for the first step */
            b_vals[1] = (seq_t)1;
            current_sum = (seq_t)1;          /* a(1) = 1 */
        } else {
            /* update b_k for k = 1 .. new_n-1 according to
                b_k(new_n) = b_k(new_n-1) * k^2 * (new_n-1) / (new_n - k)     */
            uint64_t old_n = new_n - 1;
            for (uint64_t k = old_n; k > 0; --k) {
                seq_t old_val = b_vals[k];
                seq_t tmp = old_val;
                uint64_t ksq = k * k;                      /* k^2 fits in 32 bits */
                tmp = tmp * (seq_t)ksq;
                tmp = tmp * (seq_t)old_n;
                seq_t denom = (seq_t)(new_n - k);
                seq_t new_val = tmp / denom;
                b_vals[k] = new_val;
                current_sum += new_val - old_val;          /* adjust running sum */
            }
            /* add the term for k = new_n (which is always 1) */
            b_vals[new_n] = (seq_t)1;
            current_sum += (seq_t)1;
        }

        a_vals[new_n] = current_sum;
        max_computed = new_n;
    }

    return a_vals[n];
}
