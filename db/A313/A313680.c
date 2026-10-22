/*
 * A313680  Coordination sequence Gal.6.120.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   for n ≥ 4, a(n) = 15 + 4*cnt4 + 5*cnt5, where cnt4 =
 *            6*floor((n-4)/12) + min((n-4)%12 + 1, 6), cnt5 =
 *            6*floor((n-4)/12) + max(0, (n-4)%12 - 5). The first four values
 *            are a(0)=1, a(1)=5, a(2)=10, a(3)=15
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms explicitly */
    switch (n) {
        case 0: return 1;
        case 1: return 5;
        case 2: return 10;
        case 3: return 15;
    }

    /* n ≥ 4 */
    idx_t m = n - 4;          /* number of steps from a(3) to a(n) */
    idx_t q = m / 12;         /* complete periods of 12 */
    idx_t r = m % 12;         /* remaining steps within the current period */

    /* cnt4 : number of increments equal to 4 in the last 'm+1' steps */
    seq_t cnt4 = (seq_t)(6 * q + (r < 6 ? (r + 1) : 6));

    /* cnt5 : number of increments equal to 5 in the last 'm+1' steps */
    seq_t cnt5 = (seq_t)(6 * q + (r < 5 ? 0 : (r - 5)));

    return (seq_t)15 + 4 * cnt4 + 5 * cnt5;
}
