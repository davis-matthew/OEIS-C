/*
 * A314299  Coordination sequence Gal.3.3.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n) = 22*(n/5) if n mod 5 == 0 18*(n/5)+5 if n
 *            mod 5 == 1 18*(n/5)+7 if n mod 5 == 2 18*(n/5)+11 if n mod 5 ==
 *            3 18*(n/5)+13 if n mod 5 == 4
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t k = n / 5;
    switch (n % 5) {
        case 0: return (seq_t)(22 * k);
        case 1: return (seq_t)(18 * k + 5);
        case 2: return (seq_t)(18 * k + 7);
        case 3: return (seq_t)(18 * k + 11);
        case 4: return (seq_t)(18 * k + 13);
    }
    return (seq_t)0; /* unreachable */
}
