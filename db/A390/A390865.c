/*
 * A390865  The number of integers k from 1 to n such that gcd(n, k) is an
 *          exponential divisor of n.
 * https://oeis.org/A390865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 1023, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of integers k (1≤k≤n) such that gcd(n,k) is
 * an exponential divisor of n.
 * The sequence is multiplicative.  For a prime power p^e
 * a(p^e) = 1 + (p-1) * Σ_{d|e, d<e} p^{e-d-1}.
 */

#include <stdint.h>
#include <stddef.h>

/* Simple factorisation of n (uint64_t) into primes and exponents.
 * The result is stored in the arrays `pr` and `ex` which must be
 * large enough (at most 64 distinct primes for a 64‑bit integer).
 */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    *cnt = 0;
    uint64_t exp;

    /* factor 2 */
    exp = 0;
    while ((n & 1ULL) == 0) {
        ++exp;
        n >>= 1;
    }
    if (exp) {
        pr[*cnt] = 2;
        ex[*cnt] = exp;
        ++(*cnt);
    }

    /* odd factors */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                ++exp;
                n /= p;
            }
            pr[*cnt] = p;
            ex[*cnt] = exp;
            ++(*cnt);
        }
    }

    if (n > 1) {
        pr[*cnt] = n;
        ex[*cnt] = 1;
        ++(*cnt);
    }
}

/* Integer power for seq_t using exponentiation by squaring. */
static seq_t pow_int(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* not used, but keep the guard */
        return 0;

    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_uint64(N, primes, exps, &cnt);

    seq_t result = 1;   /* multiplicative identity */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e   = exps[i];
        seq_t sum = 0;

        /* sum over proper divisors d of e */
        for (uint64_t d = 1; d < e; ++d) {
            if (e % d == 0) {
                seq_t power = pow_int(p, (idx_t)(e - d - 1));
                sum += power;
            }
        }
        seq_t term = 1 + (p - 1) * sum;
        result *= term;
    }

    return result;
}
