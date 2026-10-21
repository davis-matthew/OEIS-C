/*
 * A275929  a(n) = 2*(n-1)! + n + 1.
 * https://oeis.org/A275929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n-1)! + n + 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    if (n == 0) return 0;  /* guard; never used */

    /* static cache of factorials */
    static seq_t *fact = NULL;
    static idx_t fact_capacity = 0;
    static idx_t fact_max = 1;      /* we always keep factorial[0] and factorial[1] */

    /* initialize factorial array on first call */
    if (!fact) {
        fact_capacity = 4;                     /* room for indices 0..3 */
        fact = (seq_t *)malloc(fact_capacity * sizeof(seq_t));
        if (!fact) exit(1);
        fact[0] = (seq_t)1;
        fact[1] = (seq_t)1;
    }

    /* ensure we have space for (n-1)! */
    if (fact_capacity <= n-1) {
        size_t new_cap = fact_capacity ? fact_capacity * 2 : 4;
        while (new_cap <= n-1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        fact = tmp;
        fact_capacity = (idx_t)new_cap;
    }

    /* compute missing factorials up to n-1 */
    for (idx_t i = fact_max + 1; i <= n-1; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
    }
    if (n-1 > fact_max) fact_max = n-1;

    /* compute the sequence value */
    seq_t res = fact[n-1] * (seq_t)2;
    res = res + (seq_t)n + (seq_t)1;
    return res;
}
