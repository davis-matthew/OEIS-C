/*
 * A297036  Number of pieces in the list d(m), d(m-1), ..., d(0) of base-8
 *          digits of n; see Comments.
 * https://oeis.org/A297036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of runs of equal sign in the sequence of
 *            differences between successive base‑8 digits of n, treating a
 *            zero difference as its own sign. The count starts from 1 for
 *            two‑digit numbers and 0 for single‑digit numbers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 8, the base‑8 representation has a single digit → 0 pieces. */
    if (n < 8) {
        return (seq_t)0;
    }

    /* Extract base‑8 digits into an array; most significant digit will be at
       dig[len‑1].  The array is small (≤64 bytes). */
    unsigned char dig[64];
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        dig[len++] = (unsigned char)(tmp % 8);
        tmp /= 8;
    }

    seq_t runs = 0;
    unsigned char prev = dig[len - 1];
    int prev_sign = 0;              /* placeholder */

    /* Iterate over the digit array from most to least significant.
       Compute the sign of each difference and count runs. */
    for (int i = (int)len - 2; i >= 0; --i) {
        unsigned char curr = dig[i];
        int diff = (int)curr - (int)prev;
        int sign = diff > 0 ? 1 : (diff < 0 ? -1 : 0);

        if (runs == 0) {           /* first difference */
            runs = 1;
            prev_sign = sign;
        } else if (sign != prev_sign) {
            ++runs;
            prev_sign = sign;
        }
        prev = curr;
    }
    return (seq_t)runs;
}
