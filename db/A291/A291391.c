/*
 * A291391  p-INVERT of (1,1,0,0,0,0,...), where p(S) = (1 - 6 S)^2.
 * https://oeis.org/A291391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=12*a(n-1)-24*a(n-2)-72*a(n-3)-36*a(n-4) for n>=5,
 *            a(0)=12,a(1)=120,a(2)=1080,a(3)=9180,a(4)=75168
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage of the computed values */
    static seq_t *seq = NULL;
    static idx_t   max_n = 0;
    static size_t  cap   = 0;

    /* First call: allocate and initialise the first five values */
    if (seq == NULL) {
        /* Ensure capacity for at least the first five terms, and for n if larger */
        size_t init_cap = 5;
        if ((size_t)n + 1 > init_cap)
            init_cap = (size_t)n + 1;

        seq = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!seq) abort();               /* allocation failed */
        cap = init_cap;

        seq[0] = (seq_t)12;
        seq[1] = (seq_t)120;
        seq[2] = (seq_t)1080;
        seq[3] = (seq_t)9180;
        seq[4] = (seq_t)75168;
        max_n = 4;
    }

    /* Extend the table if a larger index is requested */
    if (n > max_n) {
        /* Grow capacity if necessary */
        if (cap < (size_t)(n + 1)) {
            size_t new_cap = cap < 8 ? 8 : cap;
            while (new_cap < (size_t)(n + 1))
                new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) abort();               /* reallocation failed */
            seq = tmp;
            cap = new_cap;
        }

        /* Compute new terms using the recurrence */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* All indices are >= 5 from this point */
            seq[(size_t)i] =
                (seq_t)12 * seq[(size_t)(i - 1)]
              - (seq_t)24 * seq[(size_t)(i - 2)]
              - (seq_t)72 * seq[(size_t)(i - 3)]
              - (seq_t)36 * seq[(size_t)(i - 4)];
        }
        max_n = n;
    }

    return seq[(size_t)n];
}
