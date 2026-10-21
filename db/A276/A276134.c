/*
 * A276134  a(5n) = a(n), a(5n+1) = a(5n+2) = a(5n+3) = a(5n+4) = a(n) + 1,
 *          a(0) = 0.
 * https://oeis.org/A276134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = number of nonzero base‑5 digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) return 0;
    while (n > 0) {
        if (n % 5 != 0) ++count;
        n /= 5;
    }
    return count;
}
