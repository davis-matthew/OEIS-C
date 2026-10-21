/*
 * A292373  A binary encoding of 3-digits in base-4 representation of n.
 * https://oeis.org/A292373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the binary number whose i‑th bit (0‑based) is 1 iff the
 *            i‑th least‑significant base‑4 digit of n equals 3
 * Range:     u8 n <= 9693, u16 n <= 9693, u32 n <= 9693, u64 n <= 9693,
 *            u128 n <= 9693, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n == 0 directly, although the loop below would also give 0.
       The special case avoids the division by zero in the loop when n is 0. */
    if (n == 0)
        return 0;

    /* Find the largest power of 4 that does not exceed n */
    idx_t pow4 = 1;
    while (pow4 <= n / 4)
        pow4 *= 4;

    seq_t result = 0;
    /* Process digits from most‑significant to least‑significant */
    while (pow4 > 0) {
        idx_t digit = (n / pow4) % 4;
        result = (result << 1) | ((digit == 3) ? 1 : 0);
        pow4 /= 4;
    }
    return result;
}
