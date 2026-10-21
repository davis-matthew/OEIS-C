/*
 * A265424  a(n) = ((-1)^n - 1)/2 + 25*floor(3*n/2) - 50*floor(n/4).
 * https://oeis.org/A265424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((-1)^n - 1)/2 + 25*floor(3*n/2) - 50*floor(n/4)
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 25*floor(3*n/2) */
    seq_t t1 = ((seq_t)n * 3) / 2;
    t1 *= 25;

    /* Compute 50*floor(n/4) */
    seq_t t2 = (seq_t)n / 4;
    t2 *= 50;

    /* Result without the (-1)^n term */
    seq_t res = t1 - t2;

    /* Apply the (-1)^n term: subtract 1 if n is odd, else nothing */
    if (n & 1)
        res -= 1;

    return res;
}
