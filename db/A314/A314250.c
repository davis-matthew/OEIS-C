/*
 * A314250  Coordination sequence Gal.4.137.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 + sum_{k=2}^{n} d(k), with d(k) = 6 if ((k-2) % 8) < 6,
 *            otherwise d(k) = 5 (a(0)=1, a(1)=5)
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)5;
    /* n >= 2 */
    seq_t t = (seq_t)(n - 1u);          /* number of increments after a(1) */
    seq_t cycles = t / (seq_t)8;
    seq_t rem    = t % (seq_t)8;
    seq_t sum_cyc = cycles * (seq_t)46; /* 6*6 + 2*5 = 46 per full cycle */
    seq_t sum_rem;
    if (rem <= (seq_t)6) {
        sum_rem = rem * (seq_t)6;
    } else {
        sum_rem = (seq_t)36 + (rem - (seq_t)6) * (seq_t)5;
    }
    return (seq_t)5 + sum_cyc + sum_rem;
}
