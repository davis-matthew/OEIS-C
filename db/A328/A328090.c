/*
 * A328090  If n even then round(n^2/48), otherwise round(n^2/49).
 * https://oeis.org/A328090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n²/48) for even n, round((n+1)²/49) for odd n
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;

    if ((n & 1ULL) == 0ULL) {                /* n even */
        /* Even divisor 48 ⇒ (x + 48/2)/48 = (x + 24)/48 */
        return (sn * sn + (seq_t)24) / (seq_t)48;
    }
    else {                                   /* n odd */
        /* Odd divisor 49 ⇒ (x + 49/2) rounded down = (x + 24)/49
           use m = n + 1, which is even, so m² has the same rounding
           behaviour as for any integer. */
        seq_t m = sn + (seq_t)1;
        return (m * m + (seq_t)24) / (seq_t)49;
    }
}
