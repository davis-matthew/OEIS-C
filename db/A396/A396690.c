/*
 * A396690  Number of permutations p of [n+2] such that |p(i+1)-p(i)| <> n for
 *          all 1 <= i < n+2.
 * https://oeis.org/A396690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)! – 4·(n+1)! + 4·n! (n ≥ 2), a(0) = 2, a(1) = 0. The
 *            values are computed with factorials stored in a static cache
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached factorial table. */
static seq_t *fact = NULL;          /* pointer to factorial array */
static size_t fact_cap = 0;         /* current allocated size */
static idx_t fact_len = 0;          /* number of entries computed */

/* Ensure that fact[0..n+2] are available. */
static void ensure_factorials(idx_t n)
{
    idx_t needed = n + 3;                       /* indices 0 .. n+2 inclusive */
    if (fact_cap < (size_t)needed) {
        fact_cap = (size_t)needed;
        seq_t *tmp = (seq_t *)realloc(fact, fact_cap * sizeof(seq_t));
        fact = tmp;                            /* realloc may move the array */
    }
    if (fact_len == 0) {
        fact[0] = (seq_t)1;                    /* 0! = 1 */
        fact_len = 1;
    }
    for (idx_t i = fact_len; i < needed; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;        /* i! = (i-1)! * i */
    }
    fact_len = needed;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;              /* base value */
    if (n == 1) return (seq_t)0;              /* base value */
    ensure_factorials(n);                     /* compute factorials up to n+2 */

    seq_t result = fact[n+2];                 /* (n+2)!          */
    result -= fact[n+1] * (seq_t)4;           /* minus 4·(n+1)!  */
    result += fact[n]   * (seq_t)4;           /* plus 4·n!       */
    return result;
}
