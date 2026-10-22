/*
 * A314981  Coordination sequence Gal.6.366.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n > 0, let k = n / 6 (integer division), r = n % 6.
 *            a(n) = 32*k if r == 0 a(n) = 28*k + 5 if r == 1 a(n) = 30*k + 9
 *            if r == 2 a(n) = 28*k + 14 if r == 3 a(n) = 30*k + 21 if r == 4
 *            a(n) = 28*k + 23 if r == 5
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    idx_t k = n / 6;
    idx_t r = n % 6;
    switch (r) {
    case 0: return (seq_t)32 * (seq_t)k;
    case 1: return (seq_t)28 * (seq_t)k + (seq_t)5;
    case 2: return (seq_t)30 * (seq_t)k + (seq_t)9;
    case 3: return (seq_t)28 * (seq_t)k + (seq_t)14;
    case 4: return (seq_t)30 * (seq_t)k + (seq_t)21;
    case 5: return (seq_t)28 * (seq_t)k + (seq_t)23;
    default: return (seq_t)0; /* unreachable */
    }
}
