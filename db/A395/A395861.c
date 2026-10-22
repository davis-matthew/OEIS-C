/*
 * A395861  Triangle read by rows: T(n,k) = denominator(CF(n,k)), where
 *          CF(n,k) = (1/k!) * Sum_{j=0..k} (-1)^j * binomial(k,j) *
 *          (1+k/2-j)^n.
 * https://oeis.org/A395861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = denominator( CF(n,k) ) where CF(n,k) = (1/k!) * *
 *            Sum_{j=0..k} (-1)^j binom(k,j) (1 + k/2 - j)^n. * This code
 *            computes the nth term by converting n to the triangle * position
 *            (row r = n exponent, col k), evaluating CF as a rational *
 *            number, reducing it and returning its denominator
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 63, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   big_pow, binom, factorial, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) */
#include "../utils/factorial.h"   /* factorial(idx_t n) */
#include "../utils/gcd.h"   /* gcd(seq_t a, seq_t b) */
#include "../utils/pow2.h"   /* pow2(idx_t exp) -> seq_t */
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

/* Convert linear index into row r and column k.
   The first element of row r has index r*(r+1)/2. */
static void idx_to_row_col(idx_t idx, idx_t *row, idx_t *col)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= idx)
        ++r;
    *row = r;
    *col = idx - r * (r + 1) / 2;
}

seq_t a(idx_t n)
{
    /* Find the position (row r, column k) of a(n) in the triangle. */
    idx_t row, col;
    idx_to_row_col(n, &row, &col);
    idx_t r = row;   /* exponent in CF  */
    idx_t k = col;   /* column index */

    /* Sum contributions of each term; keep separate positive and negative parts. */
    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    for (idx_t j = 0; j <= k; ++j) {
        /* 2*base = k + 2 - 2j, integer (can be negative). */
        int64_t base_signed = (int64_t)k + 2 - 2 * (int64_t)j;
        uint64_t abs_base = (base_signed < 0) ? (uint64_t)(-base_signed)
                                               : (uint64_t)base_signed;

        seq_t bin = binom(k, j);                  /* C(k,j) */
        seq_t pow = big_pow((seq_t)abs_base, (seq_t)r); /* |base|^r */
        seq_t term_abs = bin * pow;                 /* absolute value */

        /* Determine sign: (-1)^j * sign(base^r). */
        int s1 = ((j & 1) == 0) ? 1 : -1;
        int s2 = (base_signed < 0 && (r & 1) == 1) ? -1 : 1;
        int term_sign = s1 * s2;

        if (term_sign > 0)
            pos_sum += term_abs;
        else
            neg_sum += term_abs;
    }

    /* |N| = |pos_sum - neg_sum| */
    seq_t numer_abs;
    if (pos_sum >= neg_sum)
        numer_abs = pos_sum - neg_sum;
    else
        numer_abs = neg_sum - pos_sum;

    /* If numerator is zero, CF = 0, denominator defined as 1. */
    if (numer_abs == 0)
        return 1;

    /* Original denominator before reduction: k! * 2^r */
    seq_t denom_orig = factorial(k) * pow2(r);

    /* Reduce the fraction. */
    seq_t g = gcd(numer_abs, denom_orig);
    return denom_orig / g;
}
