/*
 * A361983  a(n) = 1 + Sum_{k=2..n} (-1)^k * k^2 * a(floor(n/k)).
 * https://oeis.org/A361983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = 1 + Sum_{k=2..n} (-1)^k * k^2 * a(floor(n/k))
 * Range:     i8 n <= 7, i16 n <= 55, i32 n <= 2047, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Cache for already computed values.  The array indices correspond
   directly to the sequence index (a(1) is stored at a_cache[1]).  */
static seq_t *a_cache = NULL;
static unsigned char *a_flag = NULL;        /* 0 = uncomputed, 1 = computed */
static size_t       a_cap  = 0;             /* allocated capacity (>= max index+1) */

/* Ensure that the cache can hold values up to index n.  The size is
   doubled until it is large enough; freshly allocated slots are
   zeroed. */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n + 1 <= a_cap)
        return;
    size_t new_cap = a_cap ? a_cap * 2 : 1;
    while (new_cap <= (size_t)n + 1)
        new_cap *= 2;

    a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    a_flag  = (unsigned char*)realloc(a_flag,  new_cap * sizeof(unsigned char));

    if (a_cache && a_flag)
    {
        if (a_cap < new_cap)
        {
            memset(&a_cache[a_cap], 0, (new_cap - a_cap) * sizeof(seq_t));
            memset(&a_flag [a_cap], 0, (new_cap - a_cap) * sizeof(unsigned char));
        }
        a_cap = new_cap;
    }
}

/* Forward declaration of the recursive worker. */
static seq_t compute_a(idx_t n);

/* Public entry point (as required).  It guarantees that the value for
   any n >= 1 is computed and cached. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* unused offset, safe to return 0 */
        return (seq_t)0;

    ensure_cache_capacity(n);

    if (a_flag[n])
        return a_cache[n];

    return compute_a(n);
}

/* Recursive computation with memoisation.  For n=1 the first
   coefficient equals 1.  For n>1 the recurrence is applied. */
static seq_t compute_a(idx_t n)
{
    if (n == (idx_t)1)
    {
        a_cache[1] = (seq_t)1;
        a_flag[1]  = 1;
        return a_cache[1];
    }

    seq_t sum = (seq_t)0;
    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t term = (seq_t)k * (seq_t)k * a(n / k);  /* a(floor(n/k)) */
        if ((k & 1U) == 0)   /* even k: add */
            sum += term;
        else                /* odd k: subtract */
            sum -= term;
    }

    a_cache[n] = (seq_t)1 + sum;
    a_flag[n]  = 1;
    return a_cache[n];
}
