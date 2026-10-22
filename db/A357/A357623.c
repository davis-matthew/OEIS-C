/*
 * A357623  Skew-alternating sum of the n-th composition in standard order.
 * https://oeis.org/A357623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A357623: skew-alternating sum of the n‑th composition in standard order. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) {
        return 0;
    }

    /* positions of the set bits in reversed binary representation (1‑based)   */
    idx_t pos[64];
    int m = 0;
    idx_t t = n;
    for (idx_t i = 0; t; ++i) {
        if (t & 1) {
            pos[m++] = i + 1;
        }
        t >>= 1;
    }
    if (m == 0) {
        return 0;
    }

    /* compute the differences that form the composition parts            */
    idx_t parts[64];
    parts[0] = pos[0];
    for (int i = 1; i < m; ++i) {
        parts[i] = pos[i] - pos[i - 1];
    }

    /* traverse the reversed composition, accumulating the skew‑alternating sum */
    for (int rev = 0; rev < m; ++rev) {
        seq_t part = (seq_t)parts[m - 1 - rev];
        /* sign pattern: + for indices 0 or 3 mod 4, - otherwise                 */
        if ((rev & 3) == 0 || (rev & 3) == 3) {
            sum += part;
        } else {
            sum -= part;
        }
    }

    return sum;
}
