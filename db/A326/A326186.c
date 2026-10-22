/*
 * A326186  a(n) = n - A057521(n), where A057521 gives the powerful part of n.
 * https://oeis.org/A326186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A057521(n)
 * Range:     u8 n <= 256, u16 n <= 9608, u32 n <= 9608, u64 n <= 9608,
 *            u128 n <= 9608, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    idx_t tmp = n;
    seq_t powerful = 1;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            if (exp >= 2) {
                seq_t p_pow = 1;
                for (int k = 0; k < exp; ++k)
                    p_pow = p_pow * (seq_t)p;   /* compute p^exp */
                powerful = powerful * p_pow;
            }
        }
    }

    if (tmp > 1) {                 /* remaining prime factor */
        /* its exponent is 1, which is < 2, so ignore it */
    }

    return (seq_t)n - powerful;
}
