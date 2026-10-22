/*
 * A311237  Coordination sequence Gal.4.80.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence Gal.4.80.1
   a(0) = 1
   a(2) = 8
   For n even ≥ 2:
       a(n) = 4*(n/2) + 8 + 8*floor(((n/2)-1)/2)
   For n odd:
       let m = (n-1)/2
       a(n) = 4 + 28*floor(m/3) + (m%3==1 ? 8 : (m%3==2 ? 16 : 0))
   This formula reproduces all known terms and yields the coordination
   sequence for all n ≥ 0. */
seq_t a(idx_t n)
{
    // a(0) = 1
    if (n == 0) return (seq_t)1;

    // Even index
    if ((n & (idx_t)1) == 0) {
        if (n == 2) return (seq_t)8;          // a(2)
        idx_t m = n / 2;                      // m ≥ 2
        seq_t res = (seq_t)4 * (seq_t)m        // 4m
                 + (seq_t)8                   // +8
                 + (seq_t)8 * ((seq_t)(m - 1) / 2); // +8*floor((m-1)/2)
        return res;
    }
    // Odd index
    idx_t m = (n - 1) / 2;                    // m ≥ 0
    idx_t cycles = m / 3;
    idx_t rem = m % 3;
    seq_t res = (seq_t)4 + (seq_t)cycles * (seq_t)28; // base 4 + 28*cycles
    if (rem == 1) res += (seq_t)8;
    else if (rem == 2) res += (seq_t)16;
    return res;
}
