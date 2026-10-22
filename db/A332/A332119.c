/*
 * A332119  a(n) = (10^(2*n+1) - 1)/9 + 8*10^n.
 * https://oeis.org/A332119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(2*n+1) - 1)/9 + 8*10^n Equivalent to the integer
 *            consisting of n ones, a 9, then n ones. Computes it by
 *            constructing the string of 1's and adding 8*10^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Build the number 111...1 with 2*n+1 digits */
    seq_t ones = 0;
    idx_t total = (n << 1) + 1;          /* 2*n + 1 */
    for (idx_t i = 0; i < total; ++i)
        ones = ones * 10 + 1;

    /* Compute 10^n */
    seq_t pow10n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow10n *= 10;

    /* Final result: ones + 8 * 10^n */
    return ones + pow10n * 8;
}
