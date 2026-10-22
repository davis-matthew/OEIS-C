/*
 * A321773  Number of compositions of n into parts with distinct
 *          multiplicities and with exactly three parts.
 * https://oeis.org/A321773
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*floor((n-1)/2) - 2 if n mod 3 == 0, otherwise a(n) =
 *            3*floor((n-1)/2) (offset 3)
 * Range:     u8 n <= 172, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;                    /* values below the offset are undefined */
    seq_t half = ((seq_t)n - 1) / 2;         /* floor((n-1)/2) */
    seq_t val  = 3 * half;                  /* 3 * floor((n-1)/2) */
    if ((n % 3) == 0)                       /* adjust when n is a multiple of 3 */
        val -= 2;
    return val;
}
