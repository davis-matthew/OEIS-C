/*
 * A372448  a(n) is the 2-adic valuation of 1 + 3*{the n-th iterate of 27 with
 *          A371094}.
 * https://oeis.org/A372448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A371093(A372444(n)) Recurrence: a(0) = 1; for n>0 a(n) =
 *            2*a(n-1) + A371093(A372443(n)) Here A371093(k) = the 2‑adic
 *            valuation of (1 + 3*k). A372443(n) is the n‑th odd iterate of 27
 *            under the Collatz map
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// The following function returns a(n) for any n ≥ 0.
// It memoises results across calls for efficiency.
// No external symbols other than a() are defined.

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  a(0) = 1
        a(n) = 2 * a(n-1) + v2(1 + 3 * odd_iterate(n))   (n ≥ 1)
        odd_iterate(0) = 27
        odd_iterate(k+1) = next_odd(
                           odd_iterate(k) * 3 + 1 )
        where next_odd(x) removes all factors of 2 from x.   */
    
    /* Memoisation buffers ------------------------------------------- */
    static seq_t *odd_vals = NULL;   /* odd_iterate values */
    static size_t odd_cap  = 0;      /* capacity of odd_vals */
    static idx_t  odd_len  = 0;      /* number of stored odd values   */

    static seq_t *a_vals  = NULL;    /* a(n) values   */
    static size_t a_cap   = 0;       /* capacity of a_vals   */
    static idx_t  a_len   = 0;       /* number of stored a(n) values */

    /* Initialise on first call ------------------------------------- */
    if (odd_len == 0) {
        odd_cap = 64;
        odd_vals = (seq_t*)malloc(odd_cap * sizeof(seq_t));
        odd_vals[0] = (seq_t)27;          /* odd_iterate(0) */
        odd_len  = 1;

        a_cap = 64;
        a_vals = (seq_t*)malloc(a_cap * sizeof(seq_t));
        a_vals[0] = (seq_t)1;            /* a(0)   */
        a_len = 1;
    }

    /* Extend memoisation up to requested n ------------------------- */
    while (a_len <= n) {
        /* Current odd value (for a(n-1)) */
        seq_t cur = odd_vals[odd_len - 1];

        /* Compute next odd iterate */
        seq_t nxt = cur * (seq_t)3 + (seq_t)1;
        while ((nxt & (seq_t)1) == (seq_t)0) {
            nxt >>= 1;                   /* strip powers of two */
        }

        /* Store next odd iterate */
        if (odd_len == odd_cap) {
            odd_cap <<= 1;                /* double capacity */
            odd_vals = (seq_t*)realloc(odd_vals,
                                       odd_cap * sizeof(seq_t));
        }
        odd_vals[odd_len] = nxt;
        odd_len++;

        /* Compute offset = v2(1 + 3*nxt) */
        seq_t val = nxt * (seq_t)3 + (seq_t)1;
        int offset = 0;
        while ((val & (seq_t)1) == (seq_t)0) {
            val >>= 1;
            ++offset;
        }

        /* Compute a(n) */
        seq_t prev_a = a_vals[a_len - 1];
        seq_t new_a = prev_a * (seq_t)2 + (seq_t)offset;

        if (a_len == a_cap) {
            a_cap <<= 1;
            a_vals = (seq_t*)realloc(a_vals,
                                     a_cap * sizeof(seq_t));
        }
        a_vals[a_len] = new_a;
        a_len++;
    }

    return a_vals[n];
}
