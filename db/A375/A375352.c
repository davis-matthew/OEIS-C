/*
 * A375352  Numbers k such that 14*k + 2 is a square.
 * https://oeis.org/A375352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*floor((n-1)/2)^2 + 8*floor((n-1)/2) + 1 if n is odd =
 *            14*floor((n-2)/2)^2 + 20*floor((n-2)/2) + 7 if n is even
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A375352: numbers k such that 14*k + 2 is a square. */
/* The sequence is given by a(2m+1) = 14m^2+8m+1 and a(2m+2) = 14m^2+20m+7, where m >= 0. */
seq_t a(idx_t n)
{
    if (n & 1U) {                          /* odd n  =>  n = 2m+1  =>  m = n/2     */
        idx_t m = n >> 1;                  /* m = floor((n-1)/2)                               */
        seq_t mm = (seq_t)m;
        return 14 * mm * mm + 8 * mm + 1;
    } else {                               /* even n =>  n = 2m+2  =>  m = n/2 - 1   */
        idx_t m = (n >> 1) - 1;            /* m = floor((n-2)/2)                               */
        seq_t mm = (seq_t)m;
        return 14 * mm * mm + 20 * mm + 7;
    }
}
