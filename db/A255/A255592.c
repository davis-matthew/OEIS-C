/*
 * A255592  Convert n to base 7, move least significant digit to most
 *          significant digit and convert back to base 10.
 * https://oeis.org/A255592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = rotation of the least significant digit in the base‑7
 *            representation of n, then interpreted back in base‑10
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0 is a trivial fixed point. */
    if (n == 0) return (seq_t)0;

    /* Store base‑7 digits, least‑significant first. */
    unsigned char digits[64];
    idx_t len = 0;
    idx_t t = n;
    while (t > 0) {
        digits[len++] = t % 7;
        t /= 7;
    }

    /* Build the rotated number: least‑significant digit becomes the most‑significant. */
    seq_t res = (seq_t)digits[0];
    for (idx_t i = len - 1; i > 0; --i) {
        res = (res * (seq_t)7) + (seq_t)digits[i];
    }
    return res;
}
