/*
 * A311535  Coordination sequence Gal.5.53.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For n = 0,1,2,3 : a(0)=1, a(1)=4, a(2)=8, a(3)=12 For n ≥ 4:
 *            a(n) = 4*n + 4*floor((n-4)/10) + min(((n-4)%10) + 1, 4) (This
 *            follows the pattern of four differences of 5 followed by six
 *            differences of 4 in each 10‑term block.)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)12;

    /* For n ≥ 4 */
    idx_t t      = n - 4;           /* offset from the start of the pattern */
    idx_t full   = t / 10;          /* number of complete 10‑term blocks */
    idx_t rem    = t % 10;          /* remaining terms in the last block   */
    /* count of the "5" differences in the first t+1 positions */
    idx_t cnt5 = full * 4 + (rem < 3 ? rem + 1 : 4);

    return ((seq_t)n) * 4 + (seq_t)cnt5;
}
