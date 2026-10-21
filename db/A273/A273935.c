/*
 * A273935  Number of ways to arrange n women and n men around a circular
 *          table so that they can be divided into n nonintersecting pairs of
 *          1 woman and 1 man sitting side-by-side.
 * https://oeis.org/A273935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-1)! * (2^n - 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t fn   = factorial(n);
    seq_t fm1  = factorial(n - 1);
    seq_t two_pow_n = seq_pow((seq_t)2, n);
    return fn * fm1 * (two_pow_n - 1);
}
