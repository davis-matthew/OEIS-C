/*
 * A312669  Coordination sequence Gal.5.111.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; For n>0: a(n)=4 + 44*⌊(n-1)/9⌋ + prefix[(n-1) mod 9]
 *            where prefix = {0,4,10,16,20,26,32,36,40,44}, i.e. a(n) = a(n-1)
 *            + d(n) with d(n) repeating 3,4,6,6,4,6,6,4,4,4,…
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The sequence repeats a simple additive pattern after the first term. */

static const seq_t prefix[10] = {0, 4, 10, 16, 20, 26, 32, 36, 40, 44};

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t m = n - 1;          /* indices for the repeating block */
    idx_t full = m / 9;       /* number of complete cycles of length 9 */
    idx_t rem  = m % 9;       /* remaining positions in the last cycle */

    seq_t result = 4 + (seq_t)full * 44 + prefix[rem];
    return result;
}
