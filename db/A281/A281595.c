/*
 * A281595  a(n) = (n^n - 3*(n-1)^n + 3*(n-2)^n - (n-3)^n)/6.
 * https://oeis.org/A281595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^n - 3*(n-1)^n + 3*(n-2)^n - (n-3)^n)/6
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Compute a(n) for all n >= 0.  The function may be called in
 * arbitrary order and is fully self‑contained. */
seq_t a(idx_t n)
{
    /* For n < 3 the expression is zero. */
    if (n < 3)
        return 0;

    /* Compute the four powers using the provided big_pow utility. */
    seq_t n_pow  = big_pow((seq_t)n,     (seq_t)n);
    seq_t n1_pow = big_pow((seq_t)(n-1), (seq_t)n);
    seq_t n2_pow = big_pow((seq_t)(n-2), (seq_t)n);
    seq_t n3_pow = big_pow((seq_t)(n-3), (seq_t)n);

    /* numerator = n_pow - 3*n1_pow + 3*n2_pow - n3_pow */
    seq_t num = n_pow;

    seq_t tmp = n1_pow;
    tmp += tmp; /* 2*n1_pow */
    tmp += n1_pow; /* 3*n1_pow */
    num -= tmp; /* subtract 3*n1_pow */

    tmp = n2_pow;
    tmp += tmp; /* 2*n2_pow */
    tmp += n2_pow; /* 3*n2_pow */
    num += tmp; /* add 3*n2_pow */

    num -= n3_pow; /* subtract n3_pow */

    /* Divide by 6 (integer division, result is exact). */
    num /= 6;

    return num;
}
