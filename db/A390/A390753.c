/*
 * A390753  The Euler totient of the smallest number whose cube is divisible
 *          by n.
 * https://oeis.org/A390753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (p-1) · p^⌊(e-1)/3⌋ (Euler φ of the smallest m
 *            with m³|n)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 1; n = 0 is not requested */
        return 1;

    idx_t m = n;
    seq_t result = 1;

    /* factor 2 */
    if ((m & 1) == 0) {
        unsigned cnt = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++cnt;
        }
        seq_t term = 1;                           /* (2-1)=1, so only p^⌊(e-1)/3⌋ */
        int power = (int)(cnt - 1) / 3;
        for (int i = 0; i < power; ++i)
            term *= 2;
        result *= term;
    }

    /* odd prime factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            unsigned cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            seq_t term = (seq_t)(p - 1);
            int power = (int)(cnt - 1) / 3;
            for (int i = 0; i < power; ++i)
                term *= (seq_t)p;
            result *= term;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        seq_t term = (seq_t)(m - 1);   /* exponent 1 → ⌊(1-1)/3⌋ = 0 */
        result *= term;
    }

    return result;
}
