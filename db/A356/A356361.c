/*
 * A356361  a(n) = Sum_{k=0..floor(n/3)} n^k * |Stirling1(n,3*k)|.
 * https://oeis.org/A356361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} n^k * |Stirling1(n,3k)|
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the unsigned Stirling numbers of the first
 * kind using their absolute‑value recurrence and caches all values of a(n)
 * that are required.  The algorithm runs in O(n^2) time and uses only
 * integer arithmetic on the arbitrary‑precision type seq_t.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* helper: grow a seq_t array to at least @need elements */
static seq_t *grow_seq(seq_t *ptr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 1 : *cap;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(ptr, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed – terminate */
        ptr = tmp;
        *cap = new_cap;
    }
    return ptr;
}

/* --------------------------------------------------------------- */
/* main function computing a(n)                                   */
seq_t a(idx_t n)
{
    static seq_t *a_cache  = NULL;      /* cached a(i) values          */
    static size_t a_cap    = 0;          /* capacity of a_cache         */
    static idx_t  computed = 0;          /* highest n that has been computed */
    static seq_t *prev    = NULL;        /* previous row of Stirling numbers */
    static size_t prev_cap = 0;
    static seq_t *curr    = NULL;        /* current row of Stirling numbers  */
    static size_t curr_cap = 0;

    /* first call – initialise caches */
    if (!a_cache) {
        a_cache = grow_seq(a_cache, &a_cap, 1);
        a_cache[0] = (seq_t)1;          /* a(0) = 1                     */
        computed  = 0;

        prev     = grow_seq(prev, &prev_cap, 1);
        prev[0]  = (seq_t)1;             /* |Stirling1(0,0)| = 1          */
        curr     = grow_seq(curr, &curr_cap, 1);  /* will be resized later */
    }

    /* compute all needed values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {

        /* ensure the Stirling arrays are large enough for row i          */
        size_t row_cap = (size_t)i + 1;
        prev = grow_seq(prev, &prev_cap, row_cap);
        curr = grow_seq(curr, &curr_cap, row_cap);

        /* compute Stirling numbers for row i                           */
        curr[0] = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t val = prev[k-1];
            if (k <= i-1) val += (seq_t)(i-1) * prev[k];
            curr[k] = val;
        }

        /* compute a(i)                                                   */
        seq_t sum = (seq_t)0;
        seq_t pow = (seq_t)1;
        for (idx_t m = 0; m <= i; m += 3) {   /* m = 3*k                      */
            sum += pow * curr[m];
            pow *= (seq_t)i;                    /* multiply by n                */
        }

        /* store a(i) in cache                                            */
        a_cache = grow_seq(a_cache, &a_cap, (size_t)i + 1);
        a_cache[i] = sum;

        /* swap prev and curr for the next iteration                     */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;

        computed = i;
    }

    return a_cache[n];
}
