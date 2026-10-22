/*
 * A328788  Expansion of psi(x^6)^5/psi(-x^3) * (f(-x)/f(-x^4))^3 in powers of
 *          x where psi(), f() are Ramanujan theta functions.
 * https://oeis.org/A328788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(n/3) - 3*sigma(n/4) + 3*sigma(n/6) - sigma(n/12)
 *            where sigma(m) is the sum of divisors of m (taken only when m is
 *            integer)
 * Range:     i8 n <= 89, i16 n <= 9422, i32 n <= 9422, i64 n <= 9422,
 *            i128 n <= 9422, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the sum of all positive divisors of m (m > 0). */
static seq_t sigma_u64(uint64_t m)
{
    seq_t result = 1;
    uint64_t temp = m;
    uint64_t p = 2;
    for (; p * p <= temp; ++p) {
        if (temp % p == 0) {
            seq_t p_pow = 1;           /* p^k */
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                p_pow *= p;           /* now p_pow = p^k */
            }
            /* Sum for this prime power: (p^(k+1)-1)/(p-1) */
            seq_t term = ((p_pow * p) - 1) / (p - 1);
            result *= term;
        }
    }
    if (temp > 1) {                         /* remaining prime factor */
        result *= (seq_t)(temp + 1);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t res = 0;

    if (n % 3 == 0) {
        res += sigma_u64((uint64_t)(n / 3));
    }
    if (n % 4 == 0) {
        res -= 3 * sigma_u64((uint64_t)(n / 4));
    }
    if (n % 6 == 0) {
        res += 3 * sigma_u64((uint64_t)(n / 6));
    }
    if (n % 12 == 0) {
        res -= sigma_u64((uint64_t)(n / 12));
    }

    return res;
}
