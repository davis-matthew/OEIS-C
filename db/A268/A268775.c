/*
 * A268775  Number of n X 2 binary arrays with some element plus some
 *          horizontally, vertically, diagonally or antidiagonally adjacent
 *          neighbor totalling two no more than once.
 * https://oeis.org/A268775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((-1)^(1+n) + 2^(2+n) + ((-1)^n + 2^(1+n))*n) / 3
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n >= 1  (offset 1) */
    /* Handle builtin unsigned types separately to avoid
       undefined shifts and integer overflow. */
    if (sizeof(seq_t) <= 8) {
        const idx_t BITS = sizeof(seq_t) * 8ULL;

        /* 2^(n+2) and 2^(n+1) using 128‑bit workspace,
           avoiding a shift when the exponent exceeds the bit width. */
        __uint128_t pow_n2 = (n + 2 < BITS) ? (((__uint128_t)1) << (n + 2)) : 0;
        __uint128_t pow_n1 = (n + 1 < BITS) ? (((__uint128_t)1) << (n + 1)) : 0;

        /* Numerator = 2^(n+2) + n*2^(n+1) + (-1)^n * n + (-1)^(n+1) */
        __uint128_t num = pow_n2 + (__uint128_t)n * pow_n1;

        if ((n & 1) == 0)    /* (-1)^n  : n even → +n, odd → -n */
            num += (__uint128_t)n;
        else
            num -= (__uint128_t)n;

        if (((n + 1) & 1) == 0)   /* (-1)^(n+1) : n+1 even → +1, odd → -1 */
            num += 1;
        else
            num -= 1;

        return (seq_t)(num / 3);
    } else {
        /* seq_t is an arbitrary‑precision or 128‑bit type.
           Use the supplied pow2 routine which performs the
           calculation safely in the target type. */
        seq_t pow_n2 = pow2(n + 2);   /* 2^(n+2) */
        seq_t pow_n1 = pow2(n + 1);   /* 2^(n+1) */

        seq_t num = pow_n2 + ((seq_t)n) * pow_n1;

        if ((n & 1) == 0)
            num += (seq_t)n;
        else
            num -= (seq_t)n;

        if (((n + 1) & 1) == 0)
            num += (seq_t)1;
        else
            num -= (seq_t)1;

        return num / 3;
    }
}
