/*
 * A375294  Number of winning combinations in the game Icosahedron Bingo after
 *          n rolls.
 * https://oeis.org/A375294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*17^(n-1) + 64*16^(n-1) - 160*15^(n-1) + 24*14^(n-1) +
 *            182*13^(n-1) - 152*12^(n-1) + 36*11^(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast binary exponentiation (works for built‑in integral types and for arbitrary‑precision
   types that overload the arithmetic operators) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 0 */
    if (n <= 1) return 0;

    idx_t k = n - 1;          /* exponent */

    /* Compute the required powers */
    seq_t pow11 = pow_seq((seq_t)11, k);
    seq_t pow12 = pow_seq((seq_t)12, k);
    seq_t pow13 = pow_seq((seq_t)13, k);
    seq_t pow14 = pow_seq((seq_t)14, k);
    seq_t pow15 = pow_seq((seq_t)15, k);
    seq_t pow16 = pow_seq((seq_t)16, k);
    seq_t pow17 = pow_seq((seq_t)17, k);

    /* Combine the terms with the given coefficients */
    seq_t result = 0;
    result += (seq_t)6   * pow17;
    result += (seq_t)64  * pow16;
    result -= (seq_t)160 * pow15;
    result += (seq_t)24  * pow14;
    result += (seq_t)182 * pow13;
    result -= (seq_t)152 * pow12;
    result += (seq_t)36  * pow11;

    return result;
}
