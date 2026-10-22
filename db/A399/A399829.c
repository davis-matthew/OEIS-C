/*
 * A399829  The number of divisors d of n such that gcud(d, n/d) is a powerful
 *          number (A001694), where gcud is the greatest common unitary
 *          divisor.
 * https://oeis.org/A399829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product over prime powers p^e of f(e) where f(e) = e+1
 *            for e != 2; f(2) = 2. Hence a(p^2)=2, a(p^e)=e+1 for e≠2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    uint64_t temp = (uint64_t)n;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t f;
            if (e == 2)
                f = 2;
            else
                f = e + 1;
            result *= f;
        }
    }
    if (temp > 1) {
        /* remaining prime factor with exponent 1 → f(1)=2 */
        result *= 2;
    }
    return result;
}
