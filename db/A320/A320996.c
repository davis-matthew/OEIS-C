/*
 * A320996  Extremal values of Euler characteristics of polytopes.
 * https://oeis.org/A320996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, more
 * Formula:   a(n) = 1 - binom(n-1, k) where k = (n-1)/2 for n ≡ 0,1,2 (mod 4)
 *            and k = (n-2)/2 for n ≡ 3 (mod 4)
 * Range:     i8 n <= 8, i16 n <= 12, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The first two terms are special: a(1)=1, a(2)=1. */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle the pre‑offset cases explicitly. */
    if (n == 1) return 1;
    if (n == 2) return 1;

    /* For n ≥ 3 compute the binomial coefficient. */
    idx_t k = (n % 4 == 3) ? (n - 2) / 2 : (n - 1) / 2;
    seq_t b = binom(n - 1, k);      /* b = C(n-1, k) */
    return 1 - b;                   /* a(n) = 1 – b */
}
