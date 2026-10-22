/*
 * A303389  Number of ways to write n as a*(a+1)/2 + b*(b+1)/2 + 5^c + 5^d,
 *          where a,b,c,d are nonnegative integers with a <= b and c <= d.
 * https://oeis.org/A303389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 9373, u32 n <= 9373, u64 n <= 9373,
 *            u128 n <= 9373, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A303389
 *
 * a(n) = number of quadruples (a,b,c,d) of non‑negative integers with
 *        a <= b, c <= d, and
 *          n = a*(a+1)/2 + b*(b+1)/2 + 5^c + 5^d
 *
 * The implementation uses a brute‑force enumeration of triangular numbers
 * and powers of 5, which is O(√n² · k²) where k = ⌊log₅(n)⌋.
 * For n≤10000 it finishes in far less than 2 s.  All data structures
 * are lazily grown, so the function works for any n in the allowed
 * 64‑bit range and is completely independent of the order of calls.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------ */
    /*  Lazy table of triangular numbers T(a) = a*(a+1)/2           */
    /* ------------------------------------------------------------ */
    static seq_t *tri = NULL;      /* array of triangular numbers   */
    static size_t tri_cap = 0;     /* capacity of tri               */
    static size_t tri_sz = 0;      /* number of elements in tri     */

    if (!tri) {                    /* first call – initialise      */
        tri_cap = 16;
        tri = (seq_t *)malloc(tri_cap * sizeof(seq_t));
        tri[0] = (seq_t)0;         /* T(0) = 0                      */
        tri_sz = 1;
    }

    /* Grow triangular number table so that tri[tri_sz-1] > n        */
    while (tri[tri_sz-1] <= n) {
        /* Determine next a value */
        idx_t a_val = (idx_t)tri_sz;                       /* a = current size (starting at 0)  */
        seq_t next_tri = ((seq_t)a_val * ((seq_t)a_val + 1)) / 2;

        /* Ensure capacity */
        if (tri_sz == tri_cap) {
            tri_cap = tri_cap * 2;
            tri = (seq_t *)realloc(tri, tri_cap * sizeof(seq_t));
        }
        tri[tri_sz++] = next_tri;
    }

    /* ------------------------------------------------------------ */
    /*  Lazy table of powers of 5: 5^e                               */
    /* ------------------------------------------------------------ */
    static seq_t *pow5 = NULL;       /* array of 5^e                   */
    static size_t pow_cap = 0;       /* capacity of pow5               */
    static size_t pow_sz = 0;        /* number of elements in pow5     */

    if (!pow5) {                    /* first call – initialise      */
        pow_cap = 16;
        pow5 = (seq_t *)malloc(pow_cap * sizeof(seq_t));
        pow5[0] = (seq_t)1;          /* 5^0 = 1                       */
        pow_sz = 1;
    }

    /* Grow powers table so that pow5[pow_sz-1] > n                  */
    while (pow5[pow_sz-1] <= n) {
        seq_t next_pow = pow5[pow_sz-1] * (seq_t)5;           /* 5^e * 5 = 5^e+1         */

        /* Ensure capacity */
        if (pow_sz == pow_cap) {
            pow_cap = pow_cap * 2;
            pow5 = (seq_t *)realloc(pow5, pow_cap * sizeof(seq_t));
        }
        pow5[pow_sz++] = next_pow;
    }

    /* ------------------------------------------------------------ */
    /*  Main counting loop                                         */
    /* ------------------------------------------------------------ */
    seq_t ans = (seq_t)0;

    for (idx_t i = 0; i < tri_sz; ++i) {
        if (tri[i] > n) break;                     /* further values are larger */

        for (idx_t j = i; j < tri_sz; ++j) {
            seq_t t_sum = tri[i] + tri[j];
            if (t_sum > n) break;

            seq_t rem = n - t_sum;                /* remainder for powers part */

            /* Count (c,d) with c <= d and 5^c + 5^d == rem */
            for (idx_t c = 0; c < pow_sz; ++c) {
                if (pow5[c] > rem) break;
                for (idx_t d = c; d < pow_sz; ++d) {
                    seq_t p_sum = pow5[c] + pow5[d];
                    if (p_sum > rem) break;
                    if (p_sum == rem) ++ans;
                }
            }
        }
    }

    return ans;
}
