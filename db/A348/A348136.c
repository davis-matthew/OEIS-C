/*
 * A348136  Irregular triangle read by rows T(n,k) in which row n lists the
 *          first digit of every term of the trajectory of n in Collatz
 *          problem including the trajectory [1, 4, 2, 1] for n = 1.
 * https://oeis.org/A348136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) = first decimal digit of the Collatz trajectory of the
 *            integer n when the flattened triangle of first digits
 *            (row‑by‑row) is enumerated. Example rows start at n=1: 1,4,2,1;
 *            2,1; 3,1,5,1,8,4,2,1;
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache storing the flattened triangle.  */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static idx_t next_row = 1;   /* next integer to start a trajectory from */

/* Ensure the cache has at least 'needed' capacity. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Append the first digits of the Collatz trajectory starting from 'row'
   to the cache.  */
static void add_row(idx_t row)
{
    seq_t cur = (seq_t)row;
    idx_t step = 0;
    const seq_t two   = (seq_t)2;
    const seq_t three = (seq_t)3;
    const seq_t ten   = (seq_t)10;
    const seq_t one   = (seq_t)1;

    for (;; ++step) {
        /* Most significant decimal digit of cur.  */
        seq_t first = cur;
        while (first >= ten)
            first /= ten;

        /* Store the digit.  */
        ensure_cache_cap(cache_size + 1);
        cache[cache_size++] = first;

        /* Stop after reaching 1 (but keep the leading 1).  */
        if (cur == one && step > 0)
            break;

        /* Next Collatz step.  */
        if ((cur % two) != (seq_t)0)     /* odd */
            cur = cur * three + one;
        else                            /* even */
            cur = cur / two;
    }
}

/* Return a(n).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Generate rows until the cache contains at least n terms.  */
    while (cache_size < (size_t)n)
        add_row(next_row++);

    return cache[(size_t)n - 1];
}
