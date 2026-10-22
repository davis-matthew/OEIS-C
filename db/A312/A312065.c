/*
 * A312065  Coordination sequence Gal.3.10.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=8. For n≥3, a(n) = a(n-1)+c[(n-3) mod 6]
 *            where c = [5, 5, 4, 4, 4, 4]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 8;

    /* Prefix sums of one period of the difference sequence */
    static const seq_t prefix[6] = {0, 5, 10, 14, 18, 22};

    idx_t m      = n - 3;          /* n≥3 → m≥0 */
    idx_t total  = m + 1;          /* number of difference steps from 3 to n */
    idx_t cycles = total / 6;      /* complete periods */
    idx_t rem    = total % 6;      /* remainder inside a period */

    /* One period contributes 5+5+4+4+4+4 = 26 to a(n) */
    seq_t sum = (seq_t)cycles * 26 + prefix[rem];

    return (seq_t)8 + sum;         /* a(2) = 8 */
}
