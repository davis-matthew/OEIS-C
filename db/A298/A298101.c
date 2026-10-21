/*
 * A298101  Expansion of x*(1 + x)/((1 - x)*(1 - 322*x + x^2)).
 * https://oeis.org/A298101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 322*a(n-1) - a(n-2) + 2 with a(0)=0, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- Static memoization cache ------------------------------------- */
static seq_t *seq_cache = NULL;   /* array holding computed terms */
static idx_t  cache_len = 0;      /* number of terms currently stored (0..cache_len-1) */

/* Ensure seq_cache can hold indices 0..n */
static void ensure_capacity(idx_t n)
{
    idx_t required = n + 1;                /* need space for 0..n inclusive */
    if (required <= cache_len)
        return;                           /* already large enough */

    seq_t *tmp = (seq_t *)realloc(seq_cache, required * sizeof(seq_t));
    if (!tmp)
        abort();                          /* allocation failed */

    seq_cache = tmp;
    /* new elements are uninitialized; they will be filled in a() */
}

/* ---------- External function ------------------------------------------- */
seq_t a(idx_t n)
{
    /* Allocate or enlarge cache if necessary */
    ensure_capacity(n);

    /* Compute any missing terms up to n (loop may be skipped if already computed) */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
            seq_cache[0] = (seq_t)0;
        else if (i == 1)
            seq_cache[1] = (seq_t)1;
        else
        {
            /* a(i) = 322*a(i-1) - a(i-2) + 2 */
            seq_t val = (seq_t)322 * seq_cache[i - 1];
            val -= seq_cache[i - 2];
            val += (seq_t)2;
            seq_cache[i] = val;
        }
    }

    /* Update the length of stored terms */
    cache_len = n + 1;

    return seq_cache[n];
}
