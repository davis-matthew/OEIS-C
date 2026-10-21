/*
 * A268044  The odd numbers congruent to {3, 4} mod 5.
 * https://oeis.org/A268044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n - (n%2 ? 2 : 1) (OEIS: A268044)
 * Range:     u8 n <= 51, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic with the sequence type */
    seq_t N = (seq_t)n;
    seq_t val = (seq_t)5 * N;

    /* Subtract 2 for odd n, 1 for even n to get the correct term */
    if (n & 1ULL)
        val -= 2;
    else
        val -= 1;

    return val;
}
