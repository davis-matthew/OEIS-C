/*
 * A340850  Dirichlet g.f.: Sum_{n>0} a(n)/n^s = zeta(s) * zeta(s-2) /
 *          (zeta(s-1))^2.
 * https://oeis.org/A340850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e of (p^(2*e)-1)*(p-1)/(p+1)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2052, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: compute a(p^e) */
static seq_t a_prime_power(uint64_t p, uint64_t e)
{
    /* compute p^(2*e) */
    seq_t pow = 1;
    uint64_t exp = 2 * e;
    while (exp > 0) {
        pow *= (seq_t)p;
        exp--;
    }
    seq_t sub1 = pow - 1;
    seq_t mult = sub1 * (seq_t)(p - 1);
    seq_t denom = (seq_t)(p + 1);
    seq_t result = mult / denom;          /* exact division */
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    uint64_t N = (uint64_t)n;
    uint64_t temp = N;
    seq_t result = 1;

    /* factor 2 */
    uint64_t e = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        e++;
    }
    if (e > 0) {
        seq_t val = a_prime_power(2, e);
        result *= val;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            seq_t val = a_prime_power(p, e);
            result *= val;
        }
    }

    /* remaining prime */
    if (temp > 1) {
        seq_t val = a_prime_power(temp, 1);
        result *= val;
    }

    return result;
}
