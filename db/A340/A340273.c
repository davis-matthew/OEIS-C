/*
 * A340273  a(n) is the number of divisors d of n such that phi(n)/phi(lpf(n))
 *          mod phi(n)/phi(d) = 0, where phi is Euler's totient function
 *          (A000010), and lpf(n) is the least prime factor of n (A020639).
 * https://oeis.org/A340273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that phi(d) is a multiple
 *            of phi(lpf(n)), where lpf(n) is the least prime factor of n and
 *            phi is Euler's totient function
 * Range:     u8 n <= 8100, u16 n <= 8100, u32 n <= 8100, u64 n <= 8100,
 *            u128 n <= 8100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* forward declarations of helpers */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt);
static void dfs(size_t idx, uint64_t phi_cur,
                const uint64_t *primes, const uint64_t *exps,
                size_t cnt, uint64_t phi_lpf, seq_t *ans);

/* Factor n into distinct prime factors and their exponents.
   primes[i] holds the i-th prime factor, exps[i] its exponent.
   cnt is set to the number of distinct primes. */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    size_t c = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (tmp > 1) {
        primes[c] = tmp;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursive enumeration of all divisors of n.
   phi_cur is phi(current divisor).
   When all prime factors are processed, test the divisibility
   condition and increment ans if satisfied. */
static void dfs(size_t idx, uint64_t phi_cur,
                const uint64_t *primes, const uint64_t *exps,
                size_t cnt, uint64_t phi_lpf, seq_t *ans)
{
    if (idx == cnt) {
        if (phi_cur % phi_lpf == 0)
            (*ans)++;   /* seq_t supports the ++ operator */
        return;
    }

    uint64_t p   = primes[idx];
    uint64_t e   = exps[idx];
    uint64_t cur_p_pow = 1;   /* p^k, starts with p^0 = 1 */
    uint64_t phi_mult = 1;    /* phi(p^k), starts with phi(p^0)=1 */

    for (size_t k = 0; k <= e; ++k) {
        /* recurse with current divisor */
        dfs(idx + 1, phi_cur * phi_mult, primes, exps, cnt, phi_lpf, ans);

        /* prepare values for next exponent k+1 */
        if (k < e) {
            cur_p_pow *= p;                    /* p^(k+1) */
            /* phi(p^(k+1)) = p^k * (p-1) = (cur_p_pow / p) * (p-1) */
            phi_mult = (cur_p_pow / p) * (p - 1);
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* offset starts at 1; a(1) = 1 by definition */
    if (n == 0)
        return 0;          /* value for n < 1 is irrelevant */
    if (n == 1)
        return 1;

    uint64_t nn = (uint64_t)n;

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_uint64(nn, primes, exps, &cnt);

    /* Least prime factor is the first prime in the factorisation */
    uint64_t lpf      = primes[0];
    uint64_t phi_lpf  = lpf - 1;  /* phi(p) = p-1 for prime p */

    seq_t answer = 0;
    dfs(0, 1ULL, primes, exps, cnt, phi_lpf, &answer);
    return answer;
}
