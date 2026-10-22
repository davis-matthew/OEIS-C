/*
 * A335048  Minimum sum of primes (see Comments).
 * https://oeis.org/A335048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n = 1 a(n) = n^2 - n + 2 if n is odd (n > 1) a(n) =
 *            n^2 - n + 1 if n is even (n > 1)
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return 0;
    }
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn - nn;
    val += (n & 1ULL) ? 2 : 1;
    return val;
}
