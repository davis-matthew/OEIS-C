/*
 * A284859  Row sums of the Sheffer triangle (exp(x), exp(3*x)-1) given in
 *          A282629.
 * https://oeis.org/A284859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + Sum_{k=1..n} binomial(n-1,k-1) * 3^k * a(n-k),
 *            a(0) = 1. The implementation uses memoization with dynamic
 *            arrays for the a-values, powers of 3 and the current row of
 *            Pascal's triangle
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoization tables */
    static seq_t *a_vals   = NULL;
    static size_t a_cap    = 0;          /* allocated capacity for a_vals */
    static idx_t  a_max    = (idx_t)0;   /* largest n computed so far */

    static seq_t *pow3    = NULL;        /* 3^k   (k >= 0) */
    static size_t pow3_len = 0;          /* number of elements in pow3 */

    static seq_t *binom   = NULL;        /* row of Pascal's triangle */
    static size_t binom_len = 0;         /* current length of binom */

    /* ---------- initialization ---------- */
    if (!a_vals) {
        a_vals = (seq_t *)malloc(sizeof(seq_t));
        a_vals[0] = (seq_t)1;            /* a(0) = 1 */
        a_cap = 1;
        a_max = 0;

        pow3   = (seq_t *)malloc(sizeof(seq_t));
        pow3[0] = (seq_t)1;             /* 3^0 = 1 */
        pow3_len = 1;

        binom   = (seq_t *)malloc(sizeof(seq_t));
        binom[0] = (seq_t)1;            /* row 0 of Pascal’s triangle */
        binom_len = 1;
    }

    /* ---------- compute values up to requested n if needed ---------- */
    for (idx_t curr = a_max + 1; curr <= n; ++curr) {
        /* Extend pow3 table up to 3^curr. */
        while (pow3_len <= (size_t)curr) {
            pow3 = (seq_t *)realloc(pow3, (pow3_len + 1) * sizeof(seq_t));
            pow3[pow3_len] = pow3[pow3_len - 1] * (seq_t)3;
            ++pow3_len;
        }

        /* Compute the sum Σ_{k=1..curr} C(curr-1,k-1) * 3^k * a(curr-k). */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= curr; ++k) {
            seq_t comb = binom[k - 1];            /* C(curr-1,k-1)   */
            seq_t powk = pow3[(size_t)k];        /* 3^k             */
            seq_t aik  = a_vals[curr - k];       /* a(curr-k)       */
            sum += comb * powk * aik;
        }
        seq_t next = a_vals[a_max] + sum;        /* a(curr) = a(curr-1) + sum */

        /* Extend a_vals array if necessary. */
        if ((size_t)curr + 1 > a_cap) {
            a_cap = (size_t)curr + 1;
            a_vals = (seq_t *)realloc(a_vals, a_cap * sizeof(seq_t));
        }
        a_vals[curr] = next;

        /* Update binom array to the next row of Pascal's triangle. */
        /* binom currently holds row (curr-1); length = curr. */
        /* After the update it will hold row curr; length = curr+1. */
        size_t new_len = binom_len + 1;          /* curr + 1 */
        binom = (seq_t *)realloc(binom, new_len * sizeof(seq_t));
        binom[new_len - 1] = (seq_t)1;           /* C(curr,curr) = 1 */
        for (size_t i = new_len - 2; i > 0; --i) {
            binom[i] = binom[i] + binom[i - 1];
        }
        /* binom[0] remains 1. */
        binom_len = new_len;

        a_max = curr;
    }

    return a_vals[n];
}
