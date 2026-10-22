/*
 * A377205  Lexicographically earliest sequence of positive integers a(1),
 *          a(2), ... such that for any n >= 0, s(n) = Sum_{k=1..n}
 *          1/(k^2*a(k)) < 1.
 * https://oeis.org/A377205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 10, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   seq_gcd, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = floor(q / (n^2*(q-p))) + 1, where s(n-1)=p/q < 1 (s(0)=0) */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_gcd.h"   /* seq_gcd(seq_t a, seq_t b) */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b) */
#include "../utils/seq_realloc.h" /* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *A          = NULL;   /* array of a(k) values */
    static idx_t A_len       = 0;      /* number of terms stored   */
    static size_t A_cap      = 0;      /* capacity of the array    */

    static seq_t p = 0;                /* numerator of current sum */
    static seq_t q = 1;                /* denominator of current sum */
    static idx_t stored = 0;           /* largest n for which we have stored a(k) */

    /* compute terms up to requested n if necessary */
    for (idx_t i = stored + 1; i <= n; ++i) {
        /* convert index to seq_t */
        seq_t ii = (seq_t)i;

        /* ii^2 */
        seq_t ii_sq = seq_mul(ii, ii);

        /* remaining margin: q - p > 0 because s(i-1) < 1 */
        seq_t diff = q - p;

        /* denominator for floor calculation: ii_sq * diff        */
        seq_t denom_inter = seq_mul(ii_sq, diff);

        /* floor(q / denom_inter) */
        seq_t floor_val = q / denom_inter;

        /* minimal a(i) */
        seq_t ai = floor_val + (seq_t)1;

        /* store ai */
        if (A_len >= A_cap) {
            size_t new_cap = (A_cap == 0) ? 8 : A_cap << 1;
            seq_t *tmp = seq_realloc(A, A_cap, new_cap);
            if (!tmp) abort();          /* out of memory */
            A = tmp;
            A_cap = new_cap;
        }
        A[A_len++] = ai;

        /* update partial sum: p/q + 1/(ii_sq * ai) */
        seq_t denom_i = seq_mul(ii_sq, ai);          /* i^2 * a(i) */
        seq_t new_p = seq_mul(p, denom_i) + q;       /* p * denom_i + q */
        seq_t new_q = seq_mul(q, denom_i);           /* q * denom_i */

        seq_t g = seq_gcd(new_p, new_q);            /* reduce fraction */
        p = new_p / g;
        q = new_q / g;
    }

    stored = n;  /* we now have terms up to n */

    return A[n - 1];   /* zero‑based indexing in array */
}
