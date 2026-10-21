/*
 * A294170  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n) + 2*n, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4, b(2) =
 *          5, and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   lower_bound_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = a(n-1) + a(n-2) + b(n) + 2*n
    a(0) = 1, a(1) = 2
    b(0) = 3, b(1) = 4, b(2) = 5
    a(n) and b(n) are the complementary increasing sequences.   */

#include <stdlib.h>
#include "../utils/lower_bound_seq.h"

/*  The main function that returns the nth a‑term. */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *a_arr  = NULL;   /* array of a values   */
    static seq_t *b_arr  = NULL;   /* array of b values   */
    static idx_t  a_sz   = 0;  /* number of a values stored  */
    static idx_t  b_sz   = 0;  /* number of b values stored  */
    static size_t a_cap  = 0;  /* capacity of a_arr   */
    static size_t b_cap  = 0;  /* capacity of b_arr   */
    static seq_t  last_b = 0;  /* last computed b value */

    /* initialise arrays on first call */
    if (a_arr == NULL) {
        a_cap = 2;
        a_arr = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (a_arr == NULL) return 0;     /* out of memory – return 0 */
        a_arr[0] = (seq_t)1;
        a_arr[1] = (seq_t)2;
        a_sz = 2;
    }
    if (b_arr == NULL) {
        b_cap = 2;
        b_arr = (seq_t *)malloc(b_cap * sizeof(seq_t));
        if (b_arr == NULL) return 0;
        b_arr[0] = (seq_t)3;
        b_arr[1] = (seq_t)4;
        b_sz = 2;
    }
    if (last_b == 0) last_b = b_arr[1];   /* initialise last_b after b(1) */

    /* grow sequences up to n (if necessary) */
    for (idx_t k = a_sz; k <= n; ++k) {
        if (k < 2) continue;   /* a(0) and a(1) already set */

        /* -----   compute b(k)   ----- */
        seq_t candidate = last_b + 1;
        while (1) {
            idx_t pos = lower_bound_seq(a_arr, a_sz, candidate);
            if (pos < a_sz && a_arr[pos] == candidate) {
                candidate++;           /* skip an a value */
                continue;
            }
            break;                     /* candidate is not an a */
        }
        seq_t b_k = candidate;

        /* store b_k */
        if (b_sz == b_cap) {
            b_cap = (b_cap == 0) ? 2 : b_cap << 1;
            b_arr = (seq_t *)realloc(b_arr, b_cap * sizeof(seq_t));
            if (b_arr == NULL) return 0;
        }
        b_arr[b_sz++] = b_k;
        last_b = b_k;

        /* -----   compute a(k)   ----- */
        seq_t term = a_arr[k-1] + a_arr[k-2] + b_k + ((seq_t)k << 1);

        if (a_sz == a_cap) {
            a_cap = (a_cap == 0) ? 2 : a_cap << 1;
            a_arr = (seq_t *)realloc(a_arr, a_cap * sizeof(seq_t));
            if (a_arr == NULL) return 0;
        }
        a_arr[a_sz++] = term;
    }

    return a_arr[(size_t)n];
}
