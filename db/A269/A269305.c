/*
 * A269305  2^n mod 27 successively displaced by 0, 3, 6, 9, etc.
 * https://oeis.org/A269305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (2^n + 3*floor(n/18)) mod 27
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n mod 27 using binary exponentiation. */
    seq_t base   = 2;
    seq_t result = 1;
    idx_t exp = n;
    while (exp)
    {
        if (exp & 1)
            result = (result * base) % 27;
        base = (base * base) % 27;
        exp >>= 1;
    }

    /* Compute 3 * floor(n/18) modulo 27.
       Because 27 = 3 * 9, 3 * x mod 27 only depends on x mod 9. */
    seq_t t = (seq_t)((n / 18) % 9);
    seq_t offset = (t * 3) % 27;

    return (result + offset) % 27;
}
