/*
 * A396701  a(n) = Sum_{d|n} tau_3(d) * sigma(n/d).
 * https://oeis.org/A396701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} tau_3(d) * sigma(n/d)
 * Range:     u8 n <= 23, u16 n <= 1259, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(p^e) = Sum_{k=0..e} C(e-k+3,3) * p^k */
static seq_t a_prime_power(uint64_t p, idx_t e)
{
    seq_t res = 0;
    seq_t power = 1;
    for (idx_t k = 0; k <= e; ++k) {
        idx_t m = e - k;
        seq_t bin = ((seq_t)(m + 3) * (m + 2) * (m + 1)) / 6;
        res += bin * power;
        power *= p;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n<1, not expected to be called */

    seq_t result = 1;
    idx_t temp = n;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            result *= a_prime_power(p, e);
        }
    }
    if (temp > 1)  /* remaining prime factor */
        result *= a_prime_power(temp, 1);

    return result;
}
