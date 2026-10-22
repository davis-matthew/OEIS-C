/*
 * A300717  Möbius transform of A003557, n divided by its largest squarefree
 *          divisor.
 * https://oeis.org/A300717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} mu(d) * phi(d) * phi(n/d) Equivalent:
 *            multiplicative, a(p)=0, a(p^e) = (p-1)*p^(e-2) for e≥2
 * Range:     u8 n <= 1023, u16 n <= 9876, u32 n <= 9876, u64 n <= 9876,
 *            u128 n <= 9876, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    seq_t result = 1;
    idx_t tmp = n;

    /* factor 2 separately for speed */
    if ((tmp & 1) == 0) {
        int e = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1; /* divide by 2 */
            ++e;
        }
        if (e == 1)
            return 0;
        seq_t p = 2;
        seq_t pow = 1;
        for (int i = 0; i < e - 2; ++i)
            pow *= p;
        result *= (p - 1) * pow;
    }

    /* odd factors */
    for (idx_t d = 3; d <= tmp / d; d += 2) {
        if (tmp % d == 0) {
            int e = 0;
            while (tmp % d == 0) {
                tmp /= d;
                ++e;
            }
            if (e == 1)
                return 0;
            seq_t p = (seq_t)d;
            seq_t pow = 1;
            for (int i = 0; i < e - 2; ++i)
                pow *= p;
            result *= (p - 1) * pow;
        }
    }

    /* leftover prime factor > 1 (exponent 1) */
    if (tmp > 1)
        return 0;

    return result;
}
