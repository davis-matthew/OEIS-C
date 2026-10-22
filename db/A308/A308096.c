/*
 * A308096  Take all the integer-sided triangles with perimeter n and sides a,
 *          b, and c such that a <= b <= c. a(n) is the sum of all the b's.
 * https://oeis.org/A308096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the middle side b over all integer‑sided *
 *            triangles with perimeter n and a ≤ b ≤ c (where a + b + c = n).
 *            * Computation is by a direct O(n^2) enumeration of all
 *            admissible * (a, b) pairs. For each pair the triangle inequality
 *            * a + b > c ⇔ a + b ≥ floor(n/2)+1 * is tested, and if s...
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the offset is not defined, but returning 0 is harmless. */
    if (n < 1)
        return (seq_t)0;

    seq_t sum = 0;
    /* a ranges from 1 to floor(n/3). */
    idx_t a_max = n / 3;

    /* Threshold for triangle inequality: a + b ≥ floor(n/2) + 1 */
    idx_t thr = (n / 2) + 1;

    for (idx_t a = 1; a <= a_max; ++a)
    {
        /* b ranges from a to floor((n - a)/2). */
        idx_t b_max = (n - a) / 2;
        for (idx_t b = a; b <= b_max; ++b)
        {
            if (a + b >= thr)
                sum += (seq_t)b;
        }
    }

    return sum;
}
