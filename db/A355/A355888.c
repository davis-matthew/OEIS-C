/*
 * A355888  a(n) = Sum_{k=1..n} k! * floor(n/k).
 * https://oeis.org/A355888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k! * floor(n/k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* compute A355888:  a(n) = sum_{k=1..n} k! * floor(n/k) */
seq_t a(idx_t n)
{
    static seq_t *fact = NULL;   /* factorial cache: fact[i] = i! */
    static idx_t fact_size = 0;  /* current allocated size (>=1) */

    /* expand factorial cache if needed */
    if (n >= fact_size) {
        /* new size: n+1 (include 0..n) */
        idx_t new_size = n + 1;
        seq_t *new_fact = (seq_t *)realloc(fact, (size_t)new_size * sizeof(seq_t));
        if (new_fact == NULL) {
            /* allocation failed; return 0 */
            return (seq_t)0;
        }
        fact = new_fact;

        /* initialise factorials from previous size up to n */
        if (fact_size == 0) {
            fact[0] = (seq_t)1;          /* 0! = 1, not used in sum */
            fact[1] = (seq_t)1;          /* 1! = 1 */
            fact_size = 2;
        }
        for (idx_t i = fact_size; i <= n; ++i) {
            fact[i] = fact[i-1] * (seq_t)i;
        }
        fact_size = new_size;
    }

    /* compute sum_{k=1..n} fact[k] * floor(n/k) */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += fact[k] * (seq_t)(n / k);
    }
    return sum;
}
