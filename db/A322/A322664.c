/*
 * A322664  a(n) = n^2 * Sum_{p^k|n} Sum_{j=1..k} 1/p^(2*j), where p are
 *          primes dividing n with multiplicity k.
 * https://oeis.org/A322664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p^k|n} (n/p^k)^2 (sum over all prime powers dividing
 *            n)
 * Range:     u8 n <= 27, u16 n <= 395, u32 n <= 8847, u64 n <= 8847,
 *            u128 n <= 8847, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n == 1)
        return 0;

    seq_t N = (seq_t)n;          /* cast once for use in arithmetic */
    seq_t result = 0;
    idx_t tmp = n;                /* copy for factorisation   */

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            /* count exponent */
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            /* add contributions of p^1 … p^e */
            idx_t pow = p;       /* current power of p   */
            for (idx_t j = 1; j <= e; ++j) {
                seq_t d = N / (seq_t)pow;   /* n / p^j   */
                result += d * d;            /* (n/p^j)^2 */
                pow *= p;                   /* next power   */
            }
        }
    }

    /* remaining prime factor > 1, if any */
    if (tmp > 1) {
        idx_t pow = tmp;
        seq_t d = N / (seq_t)pow;   /* n / tmp = 1, but keep formula */
        result += d * d;            /* (n/tmp)^2   */
    }

    return result;
}
