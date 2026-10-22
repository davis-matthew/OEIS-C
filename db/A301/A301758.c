/*
 * A301758  Clique covering number of the n X n fiveleaper graph.
 * https://oeis.org/A301758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3-(-1)^n + 2*(n-1)*(n+1))/4 for n > 6. Base values (n =
 *            1..6): 1, 4, 9, 16, 21, 22
 * Range:     u8 n <= 11, u16 n <= 213, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[6] = {1, 4, 9, 16, 21, 22};
    if (n <= 6u)
        return base[n - 1];

    /* Compute (-1)^n as -1 for odd n, 1 for even n. */
    int s = (n & 1u) ? -1 : 1;

    seq_t term = (seq_t)(3 - s)                 /* 3 - (-1)^n   */
                + (seq_t)2 * ((seq_t)(n - 1) * (seq_t)(n + 1));
    return term / 4;
}
