/*
 * A250811  Number of (n+1) X (7+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 13, u64 n <= 33, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A250811 – Number of (n+1) × (8) arrays with the given monotonicity conditions
 *
 *  Formula (recurrence):  a(1)=2025, a(2)=8049, a(3)=29145
 *                            a(n)=6*a(n-1)-11*a(n-2)+6*a(n-3)  for n≥4
 *
 *  The function caches computed values so that repeated calls in any order
 *  are fast.  It is fully compliant with the stated build rules.
 */

#include <stdlib.h>   /* malloc, realloc */

/* Cached sequence data */
static seq_t *a_cache   = NULL;
static size_t a_cap     = 0;   /* length of allocated array */
static idx_t  a_maxval  = 0;   /* highest n that has been computed (1‑based) */

/* Ensure that a_cache can hold indices up to n (inclusive). */
static void a_ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= a_cap)
        return;

    size_t new_cap = a_cap ? a_cap * 2 : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp)          /* out of memory – abort in a safe way */
        exit(1);
    a_cache = tmp;
    a_cap   = new_cap;
}

/* Initialise the first three terms explicitly. */
static void a_init(void)
{
    if (a_maxval >= 3)
        return;
    a_ensure_capacity(3);
    a_cache[1] = (seq_t)2025;
    a_cache[2] = (seq_t)8049;
    a_cache[3] = (seq_t)29145;
    a_maxval  = 3;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    a_init();

    if ((idx_t)n > a_maxval) {
        a_ensure_capacity(n);
        const seq_t six   = (seq_t)6;
        const seq_t eleven = (seq_t)11;

        /* Compute terms from a_maxval+1 up to n using the recurrence. */
        for (idx_t i = a_maxval + 1; i <= n; ++i) {
            seq_t t1 = a_cache[i-1];
            seq_t t2 = a_cache[i-2];
            seq_t t3 = a_cache[i-3];
            seq_t val = six * t1;
            val -= eleven * t2;
            val += six * t3;
            a_cache[i] = val;
        }
        a_maxval = n;
    }

    return a_cache[n];
}
