/*
 * A277812  a(n) = the first odious number encountered when starting from k =
 *          n and iterating the map k -> A003188(A006068(k)/2).
 * https://oeis.org/A277812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n if popcount(n) is odd; otherwise a(n) = a(n/2)
 * Range:     u8 n <= 255, u16 n <= 9756, u32 n <= 9756, u64 n <= 9756,
 *            u128 n <= 9756, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;          /* start from k = n */

    while (1) {
        /* compute popcount of k */
        unsigned pc = 0;
        seq_t t = k;
        while (t) {
            t &= (t - (seq_t)1); /* remove lowest set bit */
            ++pc;
        }

        /* if popcount is odd, k is odious – it is the answer */
        if (pc & 1) return k;

        /* otherwise, continue the iteration with k >>= 1 (divide by 2) */
        k >>= 1;
    }
}
