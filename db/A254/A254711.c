/*
 * A254711  Pentagonal numbers (A000326) which are also centered square
 *          numbers (A001844).
 * https://oeis.org/A254711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+194*a(n-2)-194*a(n-3)-a(n-4)+a(n-5) with initial
 *            values a(1)=1, a(2)=5, a(3)=145, a(4)=925, a(5)=28085
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms.
       cache[0] == a(1) */
    static seq_t *cache = NULL;   /* stored terms */
    static size_t cap   = 0;      /* current allocation size */
    static size_t len   = 0;      /* number of used entries */

    /* Initialise cache with the first five terms if it is empty. */
    if (len == 0) {
        size_t init_cap = 8;               /* small default capacity */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        /* guard against allocation failure */
        if (!cache) return (seq_t)0;       /* caller shall not expect this */
        cap = init_cap;

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)145;
        cache[3] = (seq_t)925;
        cache[4] = (seq_t)28085;
        len = 5;                           /* we now have 5 terms */
    }

    /* If the requested term is already cached, return it. */
    if (n <= len)
        return cache[n-1];

    /* Grow the cache if necessary. */
    if (cap < n) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Pre‑compute the sliding window values for the recurrence.
       They are a(len), a(len-1), … a(len-4). */
    seq_t t1 = cache[len-1];               /* a(len)   */
    seq_t t2 = cache[len-2];               /* a(len-1) */
    seq_t t3 = cache[len-3];               /* a(len-2) */
    seq_t t4 = cache[len-4];               /* a(len-3) */
    seq_t t5 = (len >= 5) ? cache[len-5] : 0; /* a(len-4) or dummy */

    /* Generate terms until we reach the desired index n. */
    while (len < n) {
        seq_t next = t1 + (seq_t)194 * t2
                     - (seq_t)194 * t3 - t4 + t5;

        cache[len] = next;
        len++;

        /* Advance the sliding window. */
        t5 = t4;
        t4 = t3;
        t3 = t2;
        t2 = t1;
        t1 = next;
    }

    return cache[n-1];
}
