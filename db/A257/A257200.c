/*
 * A257200  a(n) = n*(n+1)*(n+2)*(n+3)*(n^2+3*n+26)/720.
 * https://oeis.org/A257200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n^2+3*n+26)/720
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset is 1; behaviour for n<1 is irrelevant */
    if (n == 0)
        return 0;

    seq_t t[5];
    t[0] = (seq_t)n;
    t[1] = (seq_t)n + 1;
    t[2] = (seq_t)n + 2;
    t[3] = (seq_t)n + 3;
    t[4] = (seq_t)n * (seq_t)n + 3 * (seq_t)n + 26; /* n^2 + 3*n + 26 */

    /* Cancel the denominator 720 = 2^4 * 3^2 * 5 */
    static const unsigned int factors[7] = {2, 2, 2, 2, 3, 3, 5};
    for (int f = 0; f < 7; ++f) {
        unsigned int fact = factors[f];
        for (int i = 0; i < 5; ++i) {
            if (t[i] % fact == 0) {
                t[i] /= fact;
                break;
            }
        }
    }

    seq_t res = 1;
    for (int i = 0; i < 5; ++i)
        res *= t[i];

    return res;
}
