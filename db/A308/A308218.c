/*
 * A308218  Take the integer-sided obtuse triangles with perimeter n and sides
 *          a, b and c such that a <= b <= c. a(n) is the sum of all the b's.
 * https://oeis.org/A308218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the sum of the second side b in all integer-sided
   obtuse triangles with perimeter n and sides a ≤ b ≤ c. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t aa = 1; aa <= n / 3; ++aa) {
        idx_t maxB = (n - aa) / 2;
        for (idx_t b = aa; b <= maxB; ++b) {
            idx_t c = n - aa - b;
            if (aa + b <= c) continue;            /* triangle inequality failed */
            seq_t csq = (seq_t)c * c;
            seq_t asq = (seq_t)aa * aa;
            seq_t bsq = (seq_t)b * b;
            if (csq > asq + bsq) {               /* obtuse condition */
                sum += (seq_t)b;
            }
        }
    }
    return sum;
}
