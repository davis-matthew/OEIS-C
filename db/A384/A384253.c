/*
 * A384253  a(n) = 1 + ((1+(-1)^(n-1))*(n-1)!)/(n+1).
 * https://oeis.org/A384253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + ((1+(-1)^(n-1))*(n-1)!)/(n+1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Even terms equal 1; odd terms use factorial division. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static factorial cache: fact[i] = i!  (i >= 0) */
    static seq_t *fact = NULL;
    static size_t fact_cap = 0;   /* number of stored factorials */

    /* initialise cache on first call */
    if (fact == NULL) {
        fact_cap = 1;
        fact = (seq_t *)malloc(fact_cap * sizeof(seq_t));
        if (fact == NULL) return (seq_t)0;   /* allocation failure */
        fact[0] = (seq_t)1;                  /* 0! = 1 */
    }

    /* extend cache up to index (n-1) if needed */
    if ((size_t)n > fact_cap - 1) {          /* need factorial[n-1] */
        size_t new_cap = (size_t)n;           /* allocate n entries: 0..n-1 */
        fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (fact == NULL) return (seq_t)0;   /* allocation failure */
        for (size_t i = fact_cap; i < new_cap; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;/* i! = (i-1)! * i */
        }
        fact_cap = new_cap;
    }

    /* even indices: a(2k) = 1 */
    if ((n & 1) == 0) {
        return (seq_t)1;
    }

    /* odd indices: a(2k+1) = 1 + (2k)!/(k+1)  */
    seq_t numerator   = fact[n - 1];                       /* (n-1)! */
    seq_t divisor     = (seq_t)((n + 1) / 2);              /* (n+1)/2 */
    seq_t quotient     = numerator / divisor;             /* exact division */
    return quotient + (seq_t)1;
}
