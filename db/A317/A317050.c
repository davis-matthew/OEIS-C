/*
 * A317050  a(0) = 0 and for any n >= 0, a(n+1) is obtained by changing the
 *          rightmost possible digit in the negabinary representation of a(n)
 *          so as to get a value not yet in the sequence.
 * https://oeis.org/A317050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, base
 * Formula:   a(n) = A053985(A003188(n)) A003188(n) = n ^ (n >> 1) (binary
 *            Gray code) a(n) = interpret the Gray bits as a negabinary number
 *            (base -2)
 * Range:     i8 n <= 191, i16 n <= 9808, i32 n <= 9808, i64 n <= 9808,
 *            i128 n <= 9808, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t gray = n ^ (n >> 1);   /* Gray code of n */
    seq_t result = 0;
    seq_t base   = 1;            /* (-2)^0 */
    while (gray) {
        if (gray & 1ull) {
            result += base;
        }
        base = -base * 2;       /* Next power of -2 */
        gray >>= 1;
    }
    return result;
}
