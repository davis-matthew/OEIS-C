/*
 * A295664  Exponent of the highest power of 2 dividing number of divisors of
 *          n: a(n) = A007814(A000005(n)); 2-adic valuation of tau(n).
 * https://oeis.org/A295664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of the highest power of 2 dividing tau(n) (tau =
 *            number of divisors). Equivalent to sum over prime exponents e of
 *            v₂(e+1)
 * Range:     u8 n <= 9652, u16 n <= 9652, u32 n <= 9652, u64 n <= 9652,
 *            u128 n <= 9652, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    idx_t t = n;
    seq_t result = 0;

    /* Count exponent of 2 */
    idx_t exp = 0;
    while ((t & 1ULL) == 0) {
        t >>= 1;
        exp++;
    }
    if (exp != 0) {
        /* trailing ones of exp: number of consecutive 1-bits from LSB */
        idx_t e = exp;
        seq_t cnt = 0;
        while ((e & 1) == 1) {
            cnt++;
            e >>= 1;
        }
        result += cnt;
    }

    /* Factor remaining odd part */
    for (idx_t p = 3; p * p <= t; p += 2) {
        if (t % p != 0)
            continue;
        exp = 0;
        while (t % p == 0) {
            t /= p;
            exp++;
        }
        /* count trailing ones of exp */
        idx_t e = exp;
        seq_t cnt = 0;
        while ((e & 1) == 1) {
            cnt++;
            e >>= 1;
        }
        result += cnt;
    }

    /* If rest is a prime > 1 */
    if (t > 1) {
        /* exponent 1 -> binary "1" has one trailing 1 */
        result += 1;
    }

    return result;
}
