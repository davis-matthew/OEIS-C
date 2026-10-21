/*
 * A297241  Up-variation of the base-14 digits of n; see Comments.
 * https://oeis.org/A297241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{k>=1} max(0, d_{k-1} - d_k) where d_0,...,d_m are
 *            the base‑14 digits of n (least significant first). This sum is
 *            the up‑variation of n's base‑14 digits
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Extract the least significant base‑14 digit. */
    idx_t prev = n % 14;
    n /= 14;
    while (n > 0) {
        idx_t curr = n % 14;
        if (prev > curr) {
            sum += (seq_t)(prev - curr);
        }
        prev = curr;
        n /= 14;
    }
    return sum;
}
