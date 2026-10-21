/*
 * A281363  Smallest m>0 such that (2*n)^2 - 1 divides (2^m)^(2*n) - 1.
 * https://oeis.org/A281363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ord_{(2n)^2 - 1}(2) / gcd(ord_{(2n)^2 - 1}(2), 2n)
 * Range:     u8 n <= 8, u16 n <= 529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mod_pow.h"
#include "../utils/gcd.h"

/* sieve primes up to 20000 once */
static uint32_t __primes[2500];
static size_t __prime_cnt = 0;

static void __sieve_primes(void)
{
    if (__prime_cnt != 0) return;            /* already initialized */
    const uint32_t limit = 20000;
    bool buf[limit + 1];
    memset(buf, true, sizeof(buf));
    buf[0] = buf[1] = false;
    for (uint32_t i = 2; i * i <= limit; ++i)
        if (buf[i])
            for (uint32_t j = i * i; j <= limit; j += i)
                buf[j] = false;
    for (uint32_t i = 2; i <= limit; ++i)
        if (buf[i])
            __primes[__prime_cnt++] = i;
}

/* factor n into primes; primes[] and cnt are from the sieve */
static void __factor_uint64(uint64_t n, uint32_t *primes, size_t prime_cnt,
                            uint32_t *factors, uint32_t *exps, size_t *len)
{
    size_t idx = 0;
    uint64_t x = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= x; ++i) {
        uint32_t p = primes[i];
        if (x % p == 0) {
            uint32_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            factors[idx] = p;
            exps[idx] = e;
            ++idx;
        }
    }
    if (x > 1) {
        factors[idx] = (uint32_t)x;
        exps[idx] = 1;
        ++idx;
    }
    *len = idx;
}

/* compute φ(m) */
static uint64_t __phi_uint64(uint64_t m, uint32_t *primes, size_t prime_cnt)
{
    uint64_t result = m;
    uint64_t x = m;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= x; ++i) {
        uint32_t p = primes[i];
        if (x % p == 0) {
            result = result / p * (p - 1);
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) result = result / x * (x - 1);
    return result;
}

/* multiplicative order of a modulo mod (mod is odd) */
static uint64_t __order_uint64(uint64_t a, uint64_t mod,
                               uint32_t *primes, size_t prime_cnt)
{
    uint64_t phi = __phi_uint64(mod, primes, prime_cnt);
    uint64_t t = phi;

    /* primes of t */
    uint32_t f[2500], e[2500];
    size_t fcnt;
    __factor_uint64(t, primes, prime_cnt, f, e, &fcnt);

    for (size_t i = 0; i < fcnt; ++i) {
        uint32_t p = f[i];
        while (t % p == 0) {
            uint64_t cand = t / p;
            seq_t res = mod_pow((seq_t)a, cand, (seq_t)mod);
            if (res == 1)
                t = cand;
            else
                break;
        }
    }
    return t;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but guard for safety */

    __sieve_primes();

    uint64_t nn = (uint64_t)n;
    uint64_t D  = 4 * nn * nn - 1;          /* (2n)^2 - 1, always odd */

    uint64_t ord = __order_uint64(2, D, __primes, __prime_cnt);

    seq_t g   = gcd((seq_t)ord, (seq_t)(2 * nn));
    uint64_t g64 = (uint64_t)g;

    uint64_t m64 = ord / g64;
    return (seq_t)m64;
}
