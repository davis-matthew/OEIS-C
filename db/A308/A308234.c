/*
 * A308234  Take apart the sides of each of the integer-sided scalene
 *          triangles with perimeter n (at their vertices) and rearrange them
 *          orthogonally in 3-space so that their endpoints coincide at a
 *          single point. a(n) is the total volume of all rectangular prisms
 *          enclosed in this way.
 * https://oeis.org/A308234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor((n-1)/3)} Sum_{i=k+1..floor((n-k-1)/2)}
 *            sign(floor((i+k)/(n-i-k+1))) * i * k * (n-i-k)
 * Range:     u8 n <= 12, u16 n <= 42, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;

    idx_t kmax = (n - 1) / 3;
    if (kmax == 0)
        return 0;

    seq_t N = (seq_t)n;
    idx_t threshold = (n + 2) / 2;   /* ceil((n+1)/2) */

    for (idx_t kk = 1; kk <= kmax; ++kk) {
        seq_t K = (seq_t)kk;

        idx_t i_start = kk + 1;               /* minimal i > k */
        if (threshold > kk) {
            idx_t t = threshold - kk;
            if (t > i_start)
                i_start = t;
        }

        idx_t i_max = (n - kk - 1) / 2;
        if (i_start > i_max)
            continue;

        seq_t A = (seq_t)i_start;
        seq_t B = (seq_t)i_max;

        /* Sum_{i=A..B} i = (A+B)*(B-A+1)/2 */
        seq_t S1 = (A + B) * (B - A + (seq_t)1) / ((seq_t)2);

        /* Sum_{i=A..B} i^2 = (B*(B+1)*(2B+1) - (A-1)*A*(2A-1)) / 6 */
        seq_t tmpB = B * (B + (seq_t)1) * ((((seq_t)2) * B) + (seq_t)1);
        seq_t tmpA = (A - (seq_t)1) * A * ((((seq_t)2) * A) - (seq_t)1);
        seq_t S2 = (tmpB - tmpA) / ((seq_t)6);

        seq_t inner = (N - K) * S1 - S2;

        result += K * inner;
    }

    return result;
}
