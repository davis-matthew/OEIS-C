/*
 * A311548  Coordination sequence Gal.5.54.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*n + 4*⌊(n-3)/9⌋ + [0,1,3,4,4,4,4,4,4,4][(n-3) mod 9]
 *            for n >= 4; a(0)=1, a(1)=4, a(2)=8, a(3)=12
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases for the first four terms */
    if (n == 0) return (seq_t)1;
    if (n < 4)  return (seq_t)4 * (seq_t)n;   /* 4,8,12 for n=1,2,3 */

    /* Prefix sums of the periodic increment pattern:
       inc(k) = {1,2,1,0,0,0,0,0,0} for k >= 4
       Pref[r] = sum_{i=0}^{r-1} inc(4+i)
       r runs 0..9 (the 9th element is the sum of the whole period = 4) */
    static const seq_t pref[10] = {
        0ULL, 1ULL, 3ULL, 4ULL, 4ULL, 4ULL, 4ULL, 4ULL, 4ULL, 4ULL
    };

    idx_t m   = n - 3;              /* count of indices from 4 to n inclusive    */
    idx_t full = m / 9;             /* number of complete periods                */
    idx_t rem  = m % 9;             /* remainder within the current period       */

    seq_t offset = (seq_t)full * (seq_t)4 + pref[rem];
    return (seq_t)4 * (seq_t)n + offset;
}
