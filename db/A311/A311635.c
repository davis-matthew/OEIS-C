/*
 * A311635  Coordination sequence Gal.6.234.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=8, a(3)=12. For n>=4: a(n)=12 +
 *            ((n-3)/11)*54 + h((n-3)%11) where h(r)=6*r if r<=4,
 *            h(r)=30+4*(r-5) if r>4. (increments: five 6's then six 4's)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Explicit first terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)12;

    /* n >= 4 */
    idx_t t      = n - 3;               /* number of increments from n=4 to n inclusive */
    idx_t cycles = t / 11;              /* complete blocks of 5*6 + 6*4 = 54 */
    idx_t rem    = t % 11;              /* remaining increments */

    seq_t sum = (seq_t)cycles * 54;     /* increments from complete cycles */

    if (rem <= 4) {
        sum += (seq_t)rem * 6;
    } else {
        sum += 30 + (seq_t)(rem - 5) * 4;
    }

    return (seq_t)12 + sum;             /* a(3)=12 plus all increments */
}
