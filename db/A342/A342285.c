/*
 * A342285  Coordination sequence with respect to the central vertex of a
 *          dodecagon-based tiling of the plane by copies of a certain
 *          Goldberg quadrilateral tile.
 * https://oeis.org/A342285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*A138591(n-1) for n > 1; a(0) = 1, a(1) = 6
 * Range:     u8 n <= 38, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 73 terms through n = 2406 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A138591(idx_t m)
{
    /* Return the m-th term of the sequence
       1, 1, 3, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15, 17, 18, 19, ...   */
    if (m <= 1)
        return (seq_t)1;          /* m = 0 or m = 1 -> 1 */

    idx_t target = m - 2;          /* index among numbers > 1 that are not powers of 2 */
    idx_t val = 2;                 /* start scanning from 2 */

    while (1) {
        /* Skip powers of two */
        if ((val & (val - 1)) == 0) {
            val++;
            continue;
        }

        if (target == 0)
            return (seq_t)val;    /* found the desired term */

        target--;
        val++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    seq_t t = A138591(n - 1);
    return t * (seq_t)6;
}
