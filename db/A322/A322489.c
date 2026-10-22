/*
 * A322489  Numbers k such that k^k ends with 4.
 * https://oeis.org/A322489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*n - 8 for odd n, 10*n - 2 for even n (offset 1)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)          /* offset behavior is undefined; return 0 */
        return (seq_t)0;
    idx_t val;
    if (n & 1u)           /* odd index */
        val = 10u * n - 8u;
    else                  /* even index */
        val = 10u * n - 2u;
    return (seq_t)val;
}
