/*
 * A308216  Sum of the largest side lengths of all integer-sided obtuse
 *          triangles with perimeter n.
 * https://oeis.org/A308216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sum of the largest side lengths of all integer-sided obtuse
 *            triangles with perimeter n. Implementation enumerates all
 *            possible triangles (sorted by side length) and adds the largest
 *            side for those that are obtuse
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Result sum, use seq_t for precision. */
    seq_t sum = (seq_t)0;

    /* c is the largest side. Its range:
       ceil(n/3) <= c < n/2  (streightforward from a <= b <= c and
       a + b > c). */
    idx_t c_start = (n + 2) / 3;          /* ceil(n/3) */
    idx_t c_end   = (n - 1) / 2;          /* floor((n-1)/2) */

    for (idx_t c = c_start; c <= c_end; ++c) {
        idx_t S = n - c;                  /* a + b = S */

        /* a ranges so that a <= b <= c.  Equivalent to:
           max(1, S - c) <= a <= floor(S/2). */
        idx_t a_min = S > c ? (S - c) : (idx_t)1;
        idx_t a_max = S / 2;

        if (a_min > a_max) continue;

        seq_t c_sq = (seq_t)c * c;        /* reuse for each a */

        for (idx_t a = a_min; a <= a_max; ++a) {
            idx_t b = S - a;
            /* Ensure b <= c; though by construction b <= S - a_min
               ensures it, but keep explicit check for safety. */
            if (b > c) continue;

            /* Obtain squares */
            seq_t a_sq = (seq_t)a * a;
            seq_t b_sq = (seq_t)b * b;

            /* Obtuse condition: c^2 > a^2 + b^2 */
            if (c_sq > a_sq + b_sq) {
                sum += (seq_t)c;
            }
        }
    }

    return sum;
}
