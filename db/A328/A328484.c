/*
 * A328484  Dirichlet g.f.: zeta(s)^2 / (1 - 3^(-s)).
 * https://oeis.org/A328484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (e+1) if p≠3, (e+1)(e+2)/2 if p=3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t temp = (uint64_t)n;
    idx_t cnt;

    /* factor 2 */
    cnt = 0;
    while ((temp & 1ULL) == 0) { /* temp % 2 == 0 */
        temp >>= 1;
        ++cnt;
    }
    if (cnt > 0)
        result *= (seq_t)(cnt + 1);

    /* factor 3 */
    cnt = 0;
    while (temp % 3ULL == 0) {
        temp /= 3ULL;
        ++cnt;
    }
    if (cnt > 0)
        result *= ((seq_t)(cnt + 1) * (cnt + 2) / 2);

    /* remaining primes (≥5), check in 6k±1 */
    for (idx_t p = 5; p * p <= temp; p += 6) {
        /* check p */
        cnt = 0;
        while (temp % p == 0) {
            temp /= p;
            ++cnt;
        }
        if (cnt > 0)
            result *= (seq_t)(cnt + 1);

        /* check p+2 */
        idx_t p2 = p + 2;
        cnt = 0;
        while (temp % p2 == 0) {
            temp /= p2;
            ++cnt;
        }
        if (cnt > 0)
            result *= (seq_t)(cnt + 1);
    }

    /* last prime factor > sqrt(n) */
    if (temp > 1) {
        /* temp is prime; exponent is 1 */
        /* if it is 3, it would have been removed above, so multiply by 2 */
        result *= 2;
    }

    return result;
}
