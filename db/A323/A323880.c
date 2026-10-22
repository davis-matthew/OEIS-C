/*
 * A323880  Number of divisors d > 1 of n such that A003415(d) divides n,
 *          where A003415 gives the arithmetic derivative of n.
 * https://oeis.org/A323880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of divisors d > 1 of n such that A003415(d)
 *            divides n
 * Range:     u8 n <= 95, u16 n <= 9441, u32 n <= 9441, u64 n <= 9441,
 *            u128 n <= 9441, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t arithmetic_derivative(uint64_t n)
{
    if (n <= 1) return 0;
    seq_t result = 0;
    uint64_t nn = n;
    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            uint64_t e = 0;
            while (nn % p == 0) {
                ++e;
                nn /= p;
            }
            /* For factor p^e, contribution is (n/p) * e */
            result += (seq_t)(n / p) * e;
        }
    }
    if (nn > 1) {
        /* leftover prime factor */
        result += (seq_t)(n / nn);
    }
    return result;
}

/* Recursively enumerate all divisors from prime factorisation */
static void enumerate_divisors(
    const uint64_t *primes,
    const int *exps,
    int fcnt,
    int pos,
    uint64_t cur,
    idx_t orig_n,
    seq_t *ans)
{
    if (pos == fcnt) {
        if (cur > 1) {
            seq_t deriv = arithmetic_derivative(cur);
            if (deriv != 0 && orig_n % deriv == 0)
                (*ans)++;
        }
        return;
    }
    uint64_t val = cur;
    for (int e = 0; e <= exps[pos]; ++e) {
        enumerate_divisors(primes, exps, fcnt, pos + 1, val, orig_n, ans);
        val *= primes[pos];
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* Factorisation of n */
    uint64_t primes[64];
    int exps[64];
    int fcnt = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            primes[fcnt] = p;
            exps[fcnt] = (int)e;
            ++fcnt;
        }
    }
    if (m > 1) {
        primes[fcnt] = m;
        exps[fcnt] = 1;
        ++fcnt;
    }

    seq_t count = 0;
    enumerate_divisors(primes, exps, fcnt, 0, 1, n, &count);
    return count;
}
