/*
 * A292053  Wiener index of the n X n king graph.
 * https://oeis.org/A292053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1) * n * (n + 1) * (7*n^2 + 2) / 30
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast index to seq_t for all arithmetic */
    seq_t sn = (seq_t)n;

    /* Pre‑compute the factors of the formula */
    seq_t p = sn - 1;                 /* n - 1 */
    seq_t q = sn;                     /* n     */
    seq_t r = sn + 1;                 /* n + 1 */
    seq_t s = 7 * sn * sn + 2;        /* 7*n^2 + 2 */

    /* Divide by 30 = 2 * 3 * 5. Do this greedily, always
       removing one factor from the first factor that is divisible. */
    const int factors[3] = {2, 3, 5};
    for (int i = 0; i < 3; ++i) {
        int d = factors[i];
        if (p % d == 0) {
            p /= d;
        } else if (q % d == 0) {
            q /= d;
        } else if (r % d == 0) {
            r /= d;
        } else {
            s /= d;
        }
    }

    /* Multiply the remaining parts */
    seq_t result = p;
    result *= q;
    result *= r;
    result *= s;

    return result;
}
