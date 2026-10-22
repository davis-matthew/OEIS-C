/*
 * A398029  a(n) is the number of closed binary operations on n labeled
 *          elements such that the set of ordered triples (x, y, xy) contains
 *          at least one of each of the 5 kinds of ordered triple: (a,a,a),
 *          (a,a,b), (a,b,a), (b,a,a), (a,b,c), for distinct elements a,b,c.
 * https://oeis.org/A398029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^n - (n-1)^n - 1)*(n^(n^2-n) - 2*(n-1)^(n^2-n) +
 *            (n-2)^(n^2-n) - 2^(n^2-n) + 2) for n > 0 a(0) = 0 a(1) = 0 a(2)
 *            = 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    /* first factor: n^n - (n-1)^n - 1 */
    seq_t N   = (seq_t)n;
    seq_t one = 1;
    seq_t two = 2;

    seq_t power_n_n     = big_pow(N, (seq_t)n);          /* n^n */
    seq_t power_nm1_n   = big_pow(N - 1, (seq_t)n);      /* (n-1)^n */
    seq_t F1            = power_n_n - power_nm1_n - one;

    /* second factor: n^(n^2-n) - 2*(n-1)^(n^2-n) + (n-2)^(n^2-n) - 2^(n^2-n) + 2 */
    idx_t exp_idx = n * n - n;          /* n^2 - n fits in idx_t for n <= 10000 */
    seq_t exp_val = (seq_t)exp_idx;

    seq_t power_n_exp      = big_pow(N,     exp_val);   /* n^(n^2-n) */
    seq_t power_nm1_exp    = big_pow(N - 1, exp_val);   /* (n-1)^(n^2-n) */
    seq_t power_nm2_exp    = big_pow(N - 2, exp_val);   /* (n-2)^(n^2-n) */
    seq_t power_two_exp    = big_pow(two,   exp_val);   /* 2^(n^2-n) */

    seq_t two_nm1 = power_nm1_exp << 1;                 /* 2 * (n-1)^(n^2-n) */

    seq_t F2 = power_n_exp - two_nm1 + power_nm2_exp - power_two_exp + two;

    return F1 * F2;
}
