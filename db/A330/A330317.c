/*
 * A330317  a(n) = Sum_{i=0..n} r(i)*r(i+1), where r(n) = A004018(n) is the
 *          number of ways of writing n as a sum of two squares.
 * https://oeis.org/A330317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{i=0..n} r(i) * r(i+1)
 * where r(n) = A004018(n) is the number of representations of n as a sum of two squares.
 */

#include <stdint.h>
#include <stdlib.h>

/*--------------------------------------------------------------------
 * Integer square root: floor(sqrt(x))
 *--------------------------------------------------------------------*/
static idx_t isqrt(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) <= x / (r + 1))
        ++r;
    return r;
}

/*--------------------------------------------------------------------
 * Count r(n) by enumerating all lattice points (a,b) with a²+b²=n
 *--------------------------------------------------------------------*/
static seq_t count_r(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* only (0,0) */
    seq_t cnt = 0;
    idx_t sr = isqrt(n);
    for (idx_t a = 0; a <= sr; ++a)
    {
        idx_t a2 = a * a;
        idx_t b2 = n - a2;
        if (b2 > (idx_t)-1)        /* overflow guard, not needed for small n */
            continue;
        idx_t b = isqrt(b2);
        if (b * b != b2 || b < a)
            continue;             /* not a solution or already counted */

        if (a == 0 && b == 0)
            cnt += (seq_t)1;      /* (0,0) */
        else if (a == 0 || b == 0)
            cnt += (seq_t)4;      /* (±b,0) or (0,±b) */
        else if (a == b)
            cnt += (seq_t)4;      /* (±a,±a) */
        else
            cnt += (seq_t)8;      /* (±a,±b) and (±b,±a) with a≠b */
    }
    return cnt;
}

/*--------------------------------------------------------------------
 * Cache of r(n) values
 *--------------------------------------------------------------------*/
static seq_t *r_cache = NULL;    /* r values for indices 0..k */
static idx_t r_cache_limit = 0;  /* largest index currently stored */

/*--------------------------------------------------------------------
 * Ensure r_cache contains values up to 'limit' (inclusive)
 *--------------------------------------------------------------------*/
static void ensure_r_up_to(idx_t limit)
{
    if (limit <= r_cache_limit)
        return;

    size_t new_size = (size_t)(limit + 1);
    seq_t *new_r = (seq_t *)realloc(r_cache, new_size * sizeof(seq_t));
    if (!new_r)
        abort();                    /* memory exhausted */

    r_cache = new_r;

    /* compute new entries */
    for (idx_t i = r_cache_limit + 1; i <= limit; ++i)
        r_cache[i] = count_r(i);

    /* r(0) is already correct, but recompute it to be safe */
    r_cache[0] = (seq_t)1;

    r_cache_limit = limit;
}

/*--------------------------------------------------------------------
 * Main sequence function
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* need r(0)..r(n+1) */
    ensure_r_up_to(n + 1);

    seq_t result = (seq_t)0;
    for (idx_t i = 0; i <= n; ++i)
        result += r_cache[i] * r_cache[i + 1];

    return result;
}
