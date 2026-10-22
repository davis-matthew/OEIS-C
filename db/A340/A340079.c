/*
 * A340079  a(n) = n / gcd(n, 1+A018804(n)), where A018804(n) = Sum_{k=1..n}
 *          gcd(k, n).
 * https://oeis.org/A340079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / gcd(n, 1 + Sum_{k=1..n} gcd(k, n))
 * Range:     u8 n <= 59, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* --------------------------------------------------------------------- */
/* Helper: factorize n into primes[] and exponents[]                     */
/* --------------------------------------------------------------------- */
static void factor_uint64(idx_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    size_t c = 0;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            primes[c] = (uint64_t)p;
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            exps[c] = e;
            ++c;
        }
    }
    if (temp > 1) {
        primes[c] = (uint64_t)temp;
        exps[c]  = 1;
        ++c;
    }
    *cnt = c;
}

/* --------------------------------------------------------------------- */
/* Helper: Euler's totient of a divisor of n, using the factorization   */
/* --------------------------------------------------------------------- */
static seq_t phi_of_divisor(idx_t m, const uint64_t *primes, const uint64_t *exps, size_t cnt)
{
    seq_t result = (seq_t)m;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            result = result / (seq_t)p * (seq_t)(p - 1);
        }
    }
    return result;
}

/* --------------------------------------------------------------------- */
/* Recursive enumeration of all divisors of n                            */
/* --------------------------------------------------------------------- */
static void generate_divisors(idx_t n, size_t pos, seq_t cur,
                              const uint64_t *primes, const uint64_t *exps,
                              size_t cnt, seq_t *sum)
{
    if (pos == cnt) {
        /* cur is a divisor of n */
        idx_t d = (idx_t)cur;
        idx_t m = n / d;
        seq_t phi_m = phi_of_divisor(m, primes, exps, cnt);
        *sum += cur * phi_m;
        return;
    }

    /* Use the prime at position pos with exponent e = 0 .. exps[pos] */
    seq_t cur_multi = cur; /* e = 0 */
    for (uint64_t e = 0; e <= exps[pos]; ++e) {
        generate_divisors(n, pos + 1, cur_multi, primes, exps, cnt, sum);
        cur_multi *= (seq_t)primes[pos];
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)       /* behavior for n < 1 is undefined; return 0 safely */
        return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_uint64(n, primes, exps, &cnt);

    seq_t sum_gcd = 0;
    generate_divisors(n, 0, 1, primes, exps, cnt, &sum_gcd);

    seq_t g = gcd((seq_t)n, sum_gcd + 1);
    return (seq_t)n / g;
}
