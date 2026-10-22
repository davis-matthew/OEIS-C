/*
 * A334349  Total area of all r X s rectangles with integer side lengths such
 *          that r + s = n, r < s and (s - r) | (s * r).
 * https://oeis.org/A334349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{r=1..⌊(n-1)/2⌋} r·(n−r) if (n−2r) | r²
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n < 3) return 0;          /* no eligible pairs for n = 1,2 */

    idx_t limit = (n - 1) / 2;    /* maximum r with r < s */
    for (idx_t r = 1; r <= limit; ++r) {
        idx_t d = n - 2 * r;      /* d = s − r = n − 2r   */
        seq_t r_sq = (seq_t)r * r;
        if (r_sq % (seq_t)d == 0) {     /* d divides r² ?   */
            seq_t area = (seq_t)r * ((seq_t)n - r);
            sum += area;
        }
    }
    return sum;
}
