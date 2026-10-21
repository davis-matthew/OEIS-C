/*
 * A296780  Detour index of the n X n X n grid graph.
 * https://oeis.org/A296780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k-1) = 4*(k-1)^3*(4*k^2-2*k+1)^3, a(2k) =
 *            8*k^3*(32*k^6-10*k^3+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 12, u64 n <= 149, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1, but guard against 0 */

    if (n & 1) {          /* n is odd: n = 2k-1 */
        seq_t k   = ((seq_t)n + 1) / 2;
        seq_t k1  = k - 1;

        /* term1 = 4*(k-1)^3 */
        seq_t term1 = (seq_t)4;
        term1 *= k1;
        term1 *= k1;
        term1 *= k1;

        /* part  = 4*k^2 - 2*k + 1 */
        seq_t part = (seq_t)4 * k * k;
        part -= (seq_t)2 * k;
        part += 1;

        /* term2 = part^3 */
        seq_t term2 = part;
        term2 *= part;
        term2 *= part;

        return term1 * term2;
    } else {              /* n is even: n = 2k */
        seq_t k   = (seq_t)n / 2;

        /* k3 = k^3, k6 = k^6 */
        seq_t k3 = k * k * k;
        seq_t k6 = k3 * k3;

        /* part = 32*k^6 - 10*k^3 + 1 */
        seq_t part = (seq_t)32 * k6;
        part -= (seq_t)10 * k3;
        part += 1;

        /* res = 8*k^3*part */
        seq_t res = (seq_t)8 * k3;
        res *= part;

        return res;
    }
}
