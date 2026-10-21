/*
 * A279209  Length of first run of 0's in binary expansion of n.
 * https://oeis.org/A279209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the first consecutive run of 0's in the binary
 *            expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the length of the first run of 0's in the binary expansion of n. */
seq_t a(idx_t n)
{
    /* Special case for 0: its binary representation is "0", so length 1. */
    if (n == 0) return (seq_t)1;

    /* Find the index (0‑based) of the most significant set bit of n. */
    idx_t msb = 0;
    idx_t temp = n;
    while (temp > 1) {
        temp >>= 1;
        msb++;
    }

    /* Scan bits lower than the most significant one to find the first run of 0's. */
    seq_t count = 0;
    int started = 0;  /* whether the first zero has been seen */
    for (idx_t i = msb; i-- > 0;) {
        int bit = (n >> i) & 1;
        if (!started) {
            if (bit == 0) {
                started = 1;
                count = 1;
            }
        } else {
            if (bit == 0) {
                count++;
            } else { /* bit == 1, run finished */
                break;
            }
        }
    }
    return (seq_t)count;
}
