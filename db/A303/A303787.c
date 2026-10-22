/*
 * A303787  a(n) = Sum_{i=0..m} d(i)*4^i, where Sum_{i=0..m} d(i)*5^i is the
 *          base-5 representation of n.
 * https://oeis.org/A303787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=0}^{m} d(i) * 4^i, where d(i) are the base‑5 digits
 *            of n (i = 0 is the least significant digit)
 * Range:     u8 n <= 468, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow4   = 1;

    while (n > 0)
    {
        idx_t digit = n % 5;
        if (digit)
            result += (seq_t)digit * pow4;
        n   /= 5;
        pow4 *= 4;
    }

    return result;
}
