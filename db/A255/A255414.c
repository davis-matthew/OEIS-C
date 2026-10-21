/*
 * A255414  Row 4 of Ludic array A255127.
 * https://oeis.org/A255414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 210 * ((n-1)/8) + base[(n-1)%8] * where base =
 *            {7,31,59,85,113,137,163,191} is a(1..8). * This implements a(n)
 *            = a(n-8)+210 for n>8 with the given * initial values. The
 *            computation is O(1) and works * for any n >= 1. For n == 0 we
 *            return 0, though the * sequence is defined for n >= 1
 * Range:     u8 n <= 10, u16 n <= 2497, u32 n <= 9942, u64 n <= 9942,
 *            u128 n <= 9942, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[8] = {7, 31, 59, 85, 113, 137, 163, 191};

    if (n == 0)
        return (seq_t)0;

    idx_t k = (n - 1) / 8;
    seq_t result = (seq_t)210 * (seq_t)k + base[(n - 1) % 8];
    return result;
}
