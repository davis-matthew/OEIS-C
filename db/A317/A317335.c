/*
 * A317335  a(n) = A317332(n) - 8*n.
 * https://oeis.org/A317335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 20000, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 79 terms through n = 9618 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(4*n-3)=1; a(4*n-1)=-2; a(2*n)=a(n) */
seq_t a(idx_t n)
{
    idx_t m = n;
    /* remove factors of 2 */
    while ((m & 1) == 0) {
        m >>= 1;
    }
    /* odd part mod 4 determines value */
    return (m & 3) == 1 ? (seq_t)1 : (seq_t)-2;
}
