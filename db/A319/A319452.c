/*
 * A319452  Numbers that are congruent to {0, 3, 6, 10} mod 12.
 * https://oeis.org/A319452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*((n-1)/4) + [0,3,6,10][(n-1)%4]
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 3, a(3) = 6, a(4) = 10, a(5) = 12, … */

seq_t a(idx_t n)
{
    /* The offsets for the four residue classes modulo 4 */
    static const seq_t offs[4] = {0, 3, 6, 10};

    if (n == 0)          /* offset is 1 for the first term */
        return 0;

    /* Compute the integer part of (n-1)/4 and its remainder modulo 4 */
    idx_t block = (n - 1) / 4;
    idx_t rem   = (n - 1) % 4;

    /* Compute a(n) = 12*block + offs[rem] */
    seq_t result = (seq_t)12 * (seq_t)block + offs[rem];
    return result;
}
