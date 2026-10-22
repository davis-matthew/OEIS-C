/*
 * A382511  Expansion of Sum_{p prime} x^p / (1 - x^p)^3.
 * https://oeis.org/A382511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p prime | n} (n/p)*(n/p + 1)/2 (OEIS A382511)
 * Range:     u8 n <= 41, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)          /* n = 1 yields 0 */
        return 0;

    const idx_t N = n;      /* keep original n for calculations */
    idx_t temp = n;         /* copy used to strip prime factors */
    seq_t result = 0;

    /* iterate over possible prime factors up to sqrt(n) */
    for (idx_t p = 2; p <= N / p; ++p) {
        if (temp % p == 0) {
            /* add triangular number for n/p */
            idx_t m = N / p;
            seq_t t = (seq_t)m * ((seq_t)m + 1) / 2;
            result += t;

            /* strip all copies of this prime from temp */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* if a prime factor remains greater than sqrt(n) */
    if (temp > 1) {
        idx_t m = N / temp;
        seq_t t = (seq_t)m * ((seq_t)m + 1) / 2;
        result += t;
    }

    return result;
}
