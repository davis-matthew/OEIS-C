/*
 * A380249  a(n) = 4^n - binomial(n,2)*3^(n-2).
 * https://oeis.org/A380249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - binomial(n,2) * 3^(n-2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: fast exponentiation for seq_t (unsigned integer type) */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 4^n */
    seq_t pow4 = pow_seq((seq_t)4, n);

    /* binomial(n,2) */
    seq_t bin = 0;
    if (n >= 2) {
        bin = ((seq_t)n) * ((seq_t)n - 1) / 2;
    }

    /* 3^(n-2) only when n >= 2; otherwise treat as 1 but bin will be 0 */
    seq_t pow3 = 1;
    if (n >= 2) {
        pow3 = pow_seq((seq_t)3, n - 2);
    }

    /* a(n) = 4^n - binomial(n,2) * 3^(n-2) */
    return pow4 - bin * pow3;
}
