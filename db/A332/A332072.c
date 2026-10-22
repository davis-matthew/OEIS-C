/*
 * A332072  The Blue Comma sequence: Differences of indices of even terms
 *          gives back the sequence; lexicographically earliest such
 *          permutation of positive integers.
 * https://oeis.org/A332072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the constructive generation of OEIS A332072
   (Blue Comma sequence).  For every n ≥ 1, a(n) is defined by
   the algorithm below:

   1.  a(1) = 1.
   2.  Maintain the smallest unused odd number 'next_odd' (initially 1),
       and the next even number to place, its order 'even_order'
       (initially 1, so the value 2*even_order is the next even).
   3.  The index where the next even value will appear is stored in
       'next_even_pos' (initially 2).  Once an even number is placed at
       index i, the index for the following even grows by the current
       term a(k), where k is the index of the term used to compute the
       difference.  That index 'diff_index' starts at 1.
   4.  While building the sequence from 1 upwards:
          - if i == next_even_pos: place 2*even_order,
            compute the next difference using a[diff_index],
            update next_even_pos, and advance even_order and diff_index.
          - otherwise: place next_odd and increase next_odd by 2.
   5.  The sequence is computed on demand, memoising all values
       already found so that repeated calls in any order run in O(1)
       per new index. */

#include <stddef.h> /* size_t for realloc/calloc */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n==0, graceful handling */

    /* ---------- static cache ---------- */
    static seq_t *cache   = NULL;    /* 1‑based array of a(i)          */
    static idx_t  cap     = 0;       /* allocated number of slots      */
    static idx_t  len     = 0;       /* largest index that has been built */

    /* ---------- construction state ---------- */
    static idx_t  next_even_pos  = 2; /* index that will hold next even  */
    static idx_t  diff_index = 1;     /* index used for the next difference */
    static idx_t  even_order = 1;     /* order of the next even number   */
    static seq_t  next_odd   = 1;     /* smallest unused odd number      */

    /* ---------- initialise cache ---------- */
    if (!cap) {
        cap = 64; /* initial capacity, will grow as needed */
        cache = (seq_t*)calloc(cap + 1, sizeof(seq_t));
        if (!cache) return 0; /* allocation failure, unlikely in tests */
    }

    /* ---------- extend cache until it contains index n ---------- */
    if (n > len) {
        /* ensure sufficient capacity */
        if ((idx_t)n > cap) {
            idx_t new_cap = cap;
            while ((idx_t)n > new_cap) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure */
            cache = tmp;
            cap = new_cap;
        }

        /* build missing terms */
        for (idx_t i = len + 1; i <= n; ++i) {
            if (i == next_even_pos) {
                /* place the next even number */
                cache[i] = (seq_t)(2 * even_order);

                /* compute next even position using a[diff_index] */
                idx_t diff = (idx_t)cache[diff_index];
                next_even_pos = i + diff;

                /* advance indices for next even number */
                diff_index++;
                even_order++;
            } else {
                /* place the next unused odd number */
                cache[i] = next_odd;
                next_odd += (seq_t)2;
            }
        }
        len = n;
    }

    return cache[n];
}
