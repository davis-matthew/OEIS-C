/*
 * A269224  Factorial of the sum of digits of n in base 4.
 * https://oeis.org/A269224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = factorial(sum of digits of n in base 4)
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t temp = n;
    idx_t sum  = 0;
    /* sum digits in base 4 (each digit is 2 bits) */
    while (temp)
    {
        sum += temp & 3;  /* add lowest base‑4 digit */
        temp >>= 2;       /* shift by two bits */
    }

    /* factorial of the sum */
    seq_t fact = 1;
    for (idx_t i = 2; i <= sum; ++i)
        fact *= (seq_t)i;
    return fact;
}
