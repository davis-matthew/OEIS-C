/*
 * A314296  Coordination sequence Gal.4.16.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; For n>0 let k = n/7, r = n%7. a(n) = { 30*k if r==0
 *            24*k+5 if r==1 24*k+7 if r==2 24*k+11 if r==3 24*k+13 if r==4
 *            24*k+17 if r==5 24*k+19 if r==6 }
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    idx_t k = n / 7;
    int r = (int)(n % 7);
    switch (r) {
        case 0: return (seq_t)30 * k;
        case 1: return (seq_t)24 * k + (seq_t)5;
        case 2: return (seq_t)24 * k + (seq_t)7;
        case 3: return (seq_t)24 * k + (seq_t)11;
        case 4: return (seq_t)24 * k + (seq_t)13;
        case 5: return (seq_t)24 * k + (seq_t)17;
        case 6: return (seq_t)24 * k + (seq_t)19;
    }
    return (seq_t)0; /* unreachable */
}
