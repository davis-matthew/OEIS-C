/*
 * A333576  a(1) = 1; thereafter a(n) = n * uphi(n) / 2.
 * https://oeis.org/A333576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * uphi(n) / 2 (uphi is the unitary totient)
 * Range:     u8 n <= 16, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)          /* a(1) = 1 */
        return (seq_t)1;

    idx_t temp = n;
    seq_t P = 1;        /* product of (p-1) over primes dividing n */
    seq_t S = 1;        /* product of sigma(p^{e-1}) over primes */

    /* handle factor 2 separately */
    if (temp % 2 == 0) {
        int e = 0;
        while (temp % 2 == 0) {
            ++e;
            temp /= 2;
        }
        P *= (seq_t)(2 - 1);          /* (p-1) == 1 for p = 2 */
        if (e >= 2) {
            seq_t pow_pe = 1;
            for (int i = 0; i < e; ++i)
                pow_pe *= (seq_t)2;   /* compute 2^e */
            S *= (pow_pe - 1);       /* divide by (2-1)=1 */
        }
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                ++e;
                temp /= p;
            }
            P *= (seq_t)(p - 1);
            if (e >= 2) {
                seq_t pow_pe = 1;
                for (int i = 0; i < e; ++i)
                    pow_pe *= (seq_t)p;   /* compute p^e */
                S *= (pow_pe - 1) / (seq_t)(p - 1);
            }
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        seq_t p = temp;
        P *= (p - 1);            /* e == 1, sigma term contributes 1 */
    }

    seq_t res = (seq_t)n * P * S;
    res /= 2;
    return res;
}
