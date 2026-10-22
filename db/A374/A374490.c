/*
 * A374490  Greatest common divisor of sums of n consecutive cubes.
 * https://oeis.org/A374490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n if n≡1,4,5,7,8,11 (mod12) Formula: a(n) = n/2 if n≡2 or
 *            10 (mod12) Formula: a(n) = 3*n if n≡0,3,9 (mod12) Formula: a(n)
 *            = 3*n/2 if n≡6 (mod12)
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 12;

    if (r == 2 || r == 10)             /* even but not divisible by 4 or 12 */
        return (seq_t)n / 2;
    if (r == 0 || r == 3 || r == 9)    /* 0, 3, or 9 mod 12 */
        return (seq_t)n * 3;
    if (r == 6)                         /* 6 mod 12 */
        return (seq_t)n * 3 / 2;

    /* the remaining residues 1,4,5,7,8,11 */
    return (seq_t)n;
}
