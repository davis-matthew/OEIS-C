/*
 * A298215  Number of nX2 0..1 arrays with every element equal to 0, 1, 2, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A298215
    Empirical recurrence (valid for n > 6):
        a(n) = 4*a(n-1) - 5*a(n-2) + 10*a(n-3) - 24*a(n-4) + 16*a(n-5)
    Initial values:
        a(1) = 2 , a(2) = 7 , a(3) = 13 , a(4) = 29 , a(5) = 73 , a(6) = 157
    This implementation memoises the sequence in a dynamic array that
    grows as needed.  It uses the helper allocation routine
    `alloc_seq_array` to ensure proper construction of arbitrary‑precision
    types.  No C++ new/delete or malloc/realloc operators are used to
    guarantee compatibility with C and C++.
*/

#include <stddef.h>       /* size_t */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array */
#include "../utils/free_seq_array.h"    /* free_seq_array */

static seq_t *seq_cache = NULL;   /* stored values: a(1) at seq_cache[0] */
static size_t cache_cap  = 0;     /* allocated size           */
static size_t cache_len  = 0;     /* number of values stored  */

/*  Return the n-th value of the sequence, n >= 1.  For n == 0 the
    function returns zero (sequence is undefined for n <= 0). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n <= 0 */

    /* n may be larger than size_t; stop if too large to allocate */
    if (n > (idx_t)SIZE_MAX) return 0;

    size_t need = (size_t)n;          /* number of elements required */

    /* Grow cache if necessary */
    if (cache_cap < need) {
        seq_t *newarr = alloc_seq_array(need);
        if (!newarr) return 0;       /* allocation failed */

        /* copy existing values */
        for (size_t i = 0; i < cache_len; ++i) newarr[i] = seq_cache[i];

        /* free old array */
        if (seq_cache) free_seq_array(seq_cache);

        seq_cache = newarr;
        cache_cap = need;
    }

    /* Compute missing values */
    for (idx_t k = (idx_t)cache_len + 1; k <= n; ++k) {
        idx_t idx = (idx_t)(k - 1);          /* array index for a(k) */
        seq_t val;

        /* initial values */
        if (k == 1)      val = 2;
        else if (k == 2) val = 7;
        else if (k == 3) val = 13;
        else if (k == 4) val = 29;
        else if (k == 5) val = 73;
        else if (k == 6) val = 157;
        else { /* recurrence */
            seq_t t1 = 4 * seq_cache[idx - 1];       /* 4*a(k-1) */
            seq_t t2 = 5 * seq_cache[idx - 2];       /* 5*a(k-2) */
            seq_t t3 = 10 * seq_cache[idx - 3];      /*10*a(k-3) */
            seq_t t4 = 24 * seq_cache[idx - 4];      /*24*a(k-4) */
            seq_t t5 = 16 * seq_cache[idx - 5];      /*16*a(k-5) */

            val = t1 - t2 + t3 - t4 + t5;
        }

        seq_cache[idx] = val;
    }

    cache_len = (size_t)n;
    return seq_cache[n - 1];
}
