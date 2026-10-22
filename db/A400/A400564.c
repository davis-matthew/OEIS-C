/*
 * A400564  Number of S_3 orbits of nonnegative integer vectors (x, y, z) !=
 *          (0,0,0) having coordinate sum equal to n.
 * https://oeis.org/A400564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = 2 + floor(n/2) - (1 if 3|n else 0) + (round(n^2 / 48) if
 *            2|n else 0)
 * Range:     u8 n <= 99, u16 n <= 1761, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)2;               /* base term 2 */
    result += (seq_t)n / 2;                /* floor(n/2) */
    if (n % 3 == 0)                        /* subtract 1 if n divisible by 3 */
        result -= (seq_t)1;
    if (n % 2 == 0) {                      /* if n is even add round(n^2/48) */
        seq_t nn = (seq_t)n;
        result += (nn * nn + (seq_t)24) / (seq_t)48;
    }
    return result;
}
