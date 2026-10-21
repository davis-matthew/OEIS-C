/*
 * A260077  Cyclotomic polynomial value Phi(10,n!).
 * https://oeis.org/A260077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Phi(10, n!) = (n!)^4 - (n!)^3 + (n!)^2 - n! + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for factorials */
    static seq_t *fact = NULL;
    static idx_t fact_last = 0;
    static size_t fact_cap = 0;

    /* initialize factorial table with 0! = 1 and 1! = 1 */
    if (!fact) {
        fact_cap = 2;
        fact = (seq_t*)realloc(NULL, fact_cap * sizeof(seq_t));
        fact[0] = (seq_t)1;
        fact[1] = (seq_t)1;
        fact_last = 1;
    }

    /* extend factorial table up to n */
    while (fact_last < n) {
        if (fact_last + 1 >= fact_cap) {
            size_t new_cap = fact_cap ? fact_cap * 2 : 2;
            fact = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));
            fact_cap = new_cap;
        }
        idx_t next = fact_last + 1;
        fact[next] = fact[fact_last] * (seq_t)next;
        fact_last = next;
    }

    seq_t f  = fact[n];
    seq_t f2 = f * f;
    seq_t f3 = f2 * f;
    seq_t f4 = f3 * f;

    /* compute (n!)^4 - (n!)^3 + (n!)^2 - n! + 1 */
    seq_t res = f4 - f3 + f2 - f + (seq_t)1;
    return res;
}
