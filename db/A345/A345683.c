/*
 * A345683  a(n) = n! * Sum_{k=1..n} 1/floor(n/k).
 * https://oeis.org/A345683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} 1/⌊n/k⌋ (OEIS A345683)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* ---- factorial cache ---------------------------------------------------- */
static seq_t *fact_cache = NULL;   /* array of factorials 0! .. n!   */
static size_t fact_size = 0;       /* number of elements stored   */

static void ensure_factorial_cache(idx_t max_n)
{
    size_t needed = (size_t)max_n + 1;   /* need indices 0 .. max_n      */
    if (needed <= fact_size)
        return;                         /* already have it              */

    size_t new_size = fact_size ? fact_size * 2 : 16;
    if (new_size < needed)
        new_size = needed;

    seq_t *tmp = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();                          /* allocation failed            */

    if (fact_size == 0) {                 /* initialise 0! = 1           */
        tmp[0] = (seq_t)1;
        fact_size = 1;
    }

    for (size_t i = fact_size; i < new_size; ++i) {
        tmp[i] = tmp[i-1] * (seq_t)i;     /* (i)! = (i-1)! * i          */
    }

    fact_cache = tmp;
    fact_size  = new_size;
}

/* ---- sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                /* not defined in OEIS, return 0 safely */
        return 0;

    ensure_factorial_cache(n);
    seq_t fact = fact_cache[(size_t)n];

    seq_t sum = 0;
    idx_t  k   = 1;
    while (k <= n) {
        idx_t t      = n / k;              /* floor(n/k)       */
        idx_t next_k = n / t + 1;           /* first k with smaller floor */
        idx_t cnt    = next_k - k;          /* how many k share this t    */

        seq_t term = fact / (seq_t)t;       /* integer division, exact */
        term = term * (seq_t)cnt;           /* multiply by block size      */
        sum += term;
        k = next_k;                         /* jump to next block          */
    }
    return sum;
}
