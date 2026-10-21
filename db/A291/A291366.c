/*
 * A291366  The arithmetic function u(n,3,4).
 * https://oeis.org/A291366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 if n mod 3 = 0; else 4 if n mod 4 = 0; else 5 if n mod
 *            5 = 0; else 7 if n mod 7 = 0; else 9
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 3 == 0) return 3;
    if (n % 4 == 0) return 4;
    if (n % 5 == 0) return 5;
    if (n % 7 == 0) return 7;
    return 9;
}
