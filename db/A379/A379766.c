/*
 * A379766  Minimum number of kings that must be placed on an n X n chessboard
 *          such that each square is attacked or occupied by at least four
 *          kings.
 * https://oeis.org/A379766
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   if 3|n, a(n)=4*floor(n/3)^2+3*floor(n/3)+2; otherwise
 *            a(n)=4*(floor(n/3)+1)^2. (Offset: a(2) = 4)
 * Range:     u8 n <= 21, u16 n <= 381, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* values below offset are unused */
    seq_t q = (seq_t)(n / 3);      /* floor(n/3) */
    seq_t result;
    if (n % 3 == 0) {
        result = 4 * q * q + 3 * q + 2;
    } else {
        result = 4 * (q + 1) * (q + 1);
    }
    return result;
}
