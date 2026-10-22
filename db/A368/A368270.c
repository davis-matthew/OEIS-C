/*
 * A368270  a(n) = Sum_{k=0..n} 2^(n-k) * k^n.
 * https://oeis.org/A368270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * k^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *terms = NULL;        /* term[k] = 2^(current-k)*k^current */
    static seq_t *a_vals = NULL;       /* a_vals[i] = a(i) */
    static idx_t last_n = 0;
    static size_t capacity = 0;

    /* first call: initialise */
    if (terms == NULL) {
        capacity = 1;
        terms   = (seq_t *)malloc(capacity * sizeof(seq_t));
        a_vals  = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!terms || !a_vals) return 0; /* out of memory: return 0 as fallback */

        /* a(0) = 1, term[0] = 2^(0-0)*0^0 = 1 */
        terms[0] = 1;
        a_vals[0] = 1;
        last_n = 0;
    }

    /* extend cache up to n if necessary */
    if (n > last_n) {
        for (idx_t current = last_n + 1; current <= n; ++current) {
            /* ensure capacity */
            if ((size_t)(current + 1) > capacity) {
                capacity = current + 1;
                terms   = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
                a_vals  = (seq_t *)realloc(a_vals, capacity * sizeof(seq_t));
                if (!terms || !a_vals) return 0; /* out of memory */
            }

            /* update old terms: term_k^{current} = 2*k * term_k^{current-1} */
            for (idx_t k = 0; k < current; ++k) {
                seq_t mult = 2 * (seq_t)k;    /* 2*k fits in seq_t */
                terms[k] = mult * terms[k];
            }

            /* new term for k = current: 2^(current-current)*current^current = 1*current^current */
            seq_t pow_val = big_pow((seq_t)current, (seq_t)current);
            terms[current] = pow_val;

            /* compute a(current) as sum of terms[0..current] */
            seq_t sum = 0;
            for (idx_t k = 0; k <= current; ++k) {
                sum += terms[k];
            }
            a_vals[current] = sum;
        }
        last_n = n;
    }

    return a_vals[n];
}
