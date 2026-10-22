/*
 * A375786  a(n) is the minimum volume of an integer-sided cuboid having the
 *          same surface as a cube with edge length n.
 * https://oeis.org/A375786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimum volume of integer‑sided cuboid with the same surface area as a cube
   of side n.  The function searches integer triples x ≤ y ≤ z that satisfy
   xy + yz + zx = 3 n² and returns the smallest product xyz.  This
   implementation relies on the heuristic that the minimal volume occurs
   for the smallest possible x and, among those, for the smallest y.  */
seq_t a(idx_t n) {
    /* Surface half formula: xy + yz + zx = 3*n^2 */
    idx_t S = 3 * n * n;
    for (idx_t x = 1; ; ++x) {
        for (idx_t y = x; ; ++y) {
            idx_t xy = x * y;
            if (xy >= S)
                break;                  /* numerator would be ≤ 0 */
            idx_t numerator = S - xy;
            idx_t denom = x + y;
            if ((numerator % denom) == 0) {
                idx_t z = numerator / denom;
                if (z >= y) {
                    return (seq_t)x * (seq_t)y * (seq_t)z;
                }
            }
        }
    }
}
