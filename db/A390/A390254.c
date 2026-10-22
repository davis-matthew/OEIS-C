/*
 * A390254  a(0) = 2, thereafter a(n+1) is the nearest integer to 4*a(n)/3.
 * https://oeis.org/A390254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2; a(n+1) = nearest integer to 4 * a(n) / 3 (i.e.
 *            round(4*a(n)/3)). For any n ≥ 0
 * Range:     u8 n <= 16, u16 n <= 36, u32 n <= 74, u64 n <= 151,
 *            u128 n <= 305, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = 2;
    for (idx_t i = 0; i < n; ++i) {
        seq_t q = v / 3;   /* floor(v/3)   */
        seq_t r = v % 3;   /* v mod 3 (0,1,2) */
        v = q * 4 + r;     /* floor(4*v/3)  */
        if (r == 2)        /* remainder 2 → round up */
            ++v;
    }
    return v;
}
