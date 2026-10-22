/*
 * A315036  Coordination sequence Gal.4.122.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5, a(2)=9, a(3)=15, a(4)=19, and for n≥5:
 *            a(n)=a(n-1)+(n%2?7:3)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial segment explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)15;
    if (n == 4) return (seq_t)19;

    /* For n >= 5 use the derived closed form:
       a(n) = 26 + 7*odd_cnt + 3*even_cnt
       where odd_cnt = #{ odd k between 6 and n } */
    idx_t total   = n - 5;
    idx_t odd_cnt = (n & 1) ? (n - 5) / 2 : (n - 6) / 2;
    idx_t even_cnt = total - odd_cnt;

    return (seq_t)26 + (seq_t)7 * (seq_t)odd_cnt + (seq_t)3 * (seq_t)even_cnt;
}
