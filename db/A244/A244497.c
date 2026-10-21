/*
 * A244497  Number of magic labelings of the prism graph I X C_5 with magic
 *          sum n.
 * https://oeis.org/A244497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*(63+(-1)^n) + 576*n + 720*n^2 + 460*n^3 + 150*n^4 +
 *            20*n^5) / 192
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 131, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow during the computation.
       The final result is cast back to seq_t.  The case n%2==0 corresponds to
       (-1)^n = +1 and n%2==1 to (-1)^n = -1, which gives 64 or 62 respectively. */
    __uint128_t nn = n;
    __uint128_t n2 = nn * nn;
    __uint128_t n3 = n2 * nn;
    __uint128_t n4 = n3 * nn;
    __uint128_t n5 = n4 * nn;

    __uint128_t const_term = (n & 1) ? 62 : 64;   /* 63 + (-1)^n */

    __uint128_t numerator = 3 * const_term
                          + 576 * nn
                          + 720 * n2
                          + 460 * n3
                          + 150 * n4
                          + 20  * n5;

    return (seq_t)(numerator / 192);
}
