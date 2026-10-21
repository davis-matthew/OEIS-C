/*
 * A291288  a(n) = binomial(n+3, 3)*(1 + binomial(n+2, 3)/4).
 * https://oeis.org/A291288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(n+4)*(n^2-n+6)/144
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 90, u64 n <= 3719,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t for arithmetic */
    seq_t f1 = (seq_t)n + 1;
    seq_t f2 = (seq_t)n + 2;
    seq_t f3 = (seq_t)n + 3;
    seq_t f4 = (seq_t)n + 4;
    seq_t f5 = (seq_t)n * (seq_t)n - (seq_t)n + 6;

    seq_t denom = 144;            /* numerator must be divisible by 144    */
    seq_t *fac[5] = { &f1, &f2, &f3, &f4, &f5 };

    /* factor out all 2's and 3's from the factors */
    for (int i = 0; i < 5; ++i) {
        seq_t *fp = fac[i];

        /* remove factors of 2 */
        while ((denom % 2 == 0) && (*fp % 2 == 0)) {
            *fp /= 2;
            denom /= 2;
        }
        /* remove factors of 3 */
        while ((denom % 3 == 0) && (*fp % 3 == 0)) {
            *fp /= 3;
            denom /= 3;
        }
    }

    /* denom should now be 1, multiply remaining factors */
    seq_t result = f1;
    result *= f2;
    result *= f3;
    result *= f4;
    result *= f5;
    return result;
}
