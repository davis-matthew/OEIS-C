/*
 * A247968  a(n) = least k such that (k!*e^k)/(sqrt(2*Pi)*k^(k+1/2)) - 1 <
 *          1/2^n.
 * https://oeis.org/A247968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 for n <= 3, a(4)=2, a(5)=3, a(6)=6, for n >= 7, a(n) =
 *            2*a(n-1) - (n mod 2 ? 1 : 0)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Define the sequence function with external linkage */
#include <stdlib.h>

static seq_t *a_cache = NULL;        /* memoized values, 1‑based indexing */
static size_t a_cap   = 0;           /* current allocation size (index 0..cap-1) */
static size_t a_max   = 0;           /* maximum index currently computed */

seq_t a(idx_t n)
{
    /* Cast n to size_t for array indexing; values beyond 10000 are not expected. */
    size_t idx = (size_t)n;

    /* Grow cache if necessary */
    if (idx + 1 > a_cap)
    {
        size_t new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp)   /* allocation failed – unlikely in test harness */
            abort();
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* Fill the table up to idx */
    for (size_t i = a_max + 1; i <= idx; ++i)
    {
        if (i <= 3)
            a_cache[i] = (seq_t)1;
        else if (i == 4)
            a_cache[i] = (seq_t)2;
        else if (i == 5)
            a_cache[i] = (seq_t)3;
        else if (i == 6)
            a_cache[i] = (seq_t)6;
        else
        {
            seq_t prev = a_cache[i - 1];
            seq_t val  = prev + prev;              /* 2 * prev */
            if (i % 2 == 1)                       /* odd index subtract 1 */
                val = val - (seq_t)1;
            a_cache[i] = val;
        }
    }

    a_max = idx;
    return a_cache[idx];
}
