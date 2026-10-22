/*
 * A307828  Number of integer-sided triangles with perimeter n whose smallest
 *          side-length divides n.
 * https://oeis.org/A307828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer‑sided triangles with perimeter n *
 *            whose smallest side divides n
 * Range:     u8 n <= 419, u16 n <= 9250, u32 n <= 9250, u64 n <= 9250,
 *            u128 n <= 9250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t a = 1; a * 3 <= n; ++a) {          /* a ≤ n/3 */
        if (n % a != 0) continue;                 /* smallest side divides n */
        idx_t bmax = (n - a) / 2;                 /* b ≤ c  →  b ≤ (n‑a)/2 */
        if (bmax < a) continue;                  /* no valid b if upper bound < a */

        idx_t threshold = (n - 2 * a) / 2;        /* floor((n-2a)/2)  */
        idx_t bmin = threshold + 1;              /* b > (n-2a)/2      */
        if (bmin < a) bmin = a;                  /* ensure b ≥ a       */

        if (bmin <= bmax) {
            count += bmax - bmin + 1;
        }
    }
    return (seq_t)count;
}
