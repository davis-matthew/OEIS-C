/*
 * A245231  Maximum frustration of complete bipartite graph K(n,4).
 * https://oeis.org/A245231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*n/4) - 1 if n ≡ 1,4,5 (mod 8), otherwise a(n) =
 *            floor(5*n/4)
 * Range:     u8 n <= 205, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(5*n/4) using integer arithmetic */
    seq_t val = (seq_t)5 * n / 4;
    idx_t r = n % 8;

    if (r == 1 || r == 4 || r == 5)
        val--;

    return val;
}
