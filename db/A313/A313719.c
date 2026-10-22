/*
 * A313719  Coordination sequence Gal.5.116.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + 1 – c(n), where c(n) = 0 for n = 0 c(n) = 1 for 1 ≤
 *            n ≤ 4 c(n) = 1 + 2*⌊(n‑5)/10⌋ + 1 + ⌊( (n‑5) mod 10 ) ≥ 1⌋ for n
 *            ≥ 5. This counts the extra “4‑step” positions (at n = 1 and at
 *            every n with n mod 10 ∈ {5,6} starting from 5). The sequence is
 *            the coordination sequence Gal.5.116.4
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;        /* a(0) = 1 */

    /* Count how many times the step size is 4 up to index n.
       The very first 4-step occurs at n = 1.  For n ≥ 5, every
       block of 10 indices contributes two more 4-steps at the
       positions ending in 5 and 6. */
    seq_t c = (seq_t)1;           /* the 4‑step at n = 1 */

    if (n >= 5) {
        idx_t m = n - 5;            /* indices from 5 to n inclusive */
        idx_t q = m / 10;           /* number of complete 10‑index blocks */
        idx_t r = m % 10;           /* remainder within the last block   */
        c += (seq_t)(2 * q + 1 + (r >= 1 ? 1 : 0));
    }

    return (seq_t)5 * (seq_t)n + (seq_t)1 - c;
}
