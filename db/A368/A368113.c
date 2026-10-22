/*
 * A368113  Trajectory of 7 under the '7x+1' map.
 * https://oeis.org/A368113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Trajectory of 7 under the '7x+1' map
 *  a(0) = 7
 *  For m ≥ 0:
 *    if a(m) is even          -> a(m+1) = a(m) / 2
 *    else if a(m) % 5 == 0    -> a(m+1) = a(m) / 5
 *    else                     -> a(m+1) = 7 * a(m) + 1
 */
seq_t a(idx_t n) {
    seq_t x = 7;
    for (idx_t i = 0; i < n; ++i) {
        if ((x & 1) == 0) {          /* even */
            x = x / 2;
        } else if ((x % 5) == 0) {   /* odd and divisible by 5 */
            x = x / 5;
        } else {                     /* odd and not divisible by 5 */
            x = 7 * x + 1;
        }
    }
    return x;
}
