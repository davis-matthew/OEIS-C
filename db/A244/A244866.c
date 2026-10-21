/*
 * A244866  Let G denote the 7-node, 12-edge graph formed from a hexagon with
 *          main diagonals drawn and a node at the center; a(n) = number of
 *          magic labelings of G with magic sum 2n.
 * https://oeis.org/A244866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(n + 2)*(19*n^3 + 63*n^2 + 68*n + 30)/60
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 105, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t A = nn + 1;           /* n+1 */
    seq_t B = nn + 2;           /* n+2 */

    /* compute 19*n^3 + 63*n^2 + 68*n + 30 */
    seq_t n_sq = nn * nn;               /* n^2 */
    seq_t n_cu = n_sq * nn;             /* n^3 */
    seq_t C = (seq_t)19 * n_cu;
    C += (seq_t)63 * n_sq;
    C += (seq_t)68 * nn;
    C += (seq_t)30;

    /* divide out the factor 60 = 2^2 * 3 * 5 from the product */
    seq_t fac[3] = { A, B, C };
    seq_t div = 60;
    const int primes[3] = { 2, 3, 5 };
    int pi = 0;

    while (div > 1) {
        int p = primes[pi];
        for (int i = 0; i < 3; ++i) {
            while (div % p == 0 && fac[i] % p == 0) {
                fac[i] /= p;
                div /= p;
            }
        }
        if (div % p != 0) ++pi;
    }

    seq_t res = fac[0];
    res *= fac[1];
    res *= fac[2];
    return res;
}
