/*
 * A366951  a(n) = 2*(p_n - 1)/A060305(n) iff p_n == +/- 1 (mod 5), 2*(p_n +
 *          1)/A060305(n) iff p_n == +/- 2 (mod 5), 0 iff p_n = 5.
 * https://oeis.org/A366951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(p_n - 1)/A060305(n) if p_n ≡ ±1 (mod 5), * 2*(p_n +
 *            1)/A060305(n) if p_n ≡ ±2 (mod 5), * 0 if p_n = 5. * Here p_n is
 *            the n‑th prime and A060305(n) is the Pisano period of *
 *            Fibonacci numbers modulo p_n
 * Range:     u8 n <= 30, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* ---------- prime storage ---------- */
static uint64_t *prime_list = NULL;    /* array of primes */
static size_t   prime_cnt  = 0;        /* number of primes stored */

/* ensure that we have at least n primes in prime_list */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;

    /* start with a safe upper bound: n * 25 + 100 */
    idx_t limit = n * 25 + 100;
    uint64_t *plist = NULL;
    size_t cnt = 0;
    build_primes_upto(limit, &plist, &cnt);
    while (cnt < (size_t)n) {
        /* not enough primes – double the limit and try again */
        free(plist);
        limit *= 2;
        build_primes_upto(limit, &plist, &cnt);
    }

    /* replace the old list */
    free(prime_list);
    prime_list = plist;
    prime_cnt  = cnt;
}

/* ---------- fast doubling Fibonacci modulo mod ---------- */
static void fib_mod(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    uint64_t a, b, c, d;
    fib_mod(n >> 1, mod, &a, &b);
    /* a = F(k), b = F(k+1) */
    uint64_t a_mod = a % mod;
    uint64_t b_mod = b % mod;

    /* F(2k) = a * (2*b - a) */
    uint64_t two_b = (b_mod << 1) % mod;
    uint64_t term  = (two_b + mod - a_mod) % mod;
    c = (a_mod * term) % mod;

    /* F(2k+1) = a^2 + b^2 */
    d = ( (a_mod * a_mod) % mod + (b_mod * b_mod) % mod ) % mod;

    if (n & 1) {            /* odd */
        *fn  = d;
        *fn1 = (c + d) % mod;
    } else {                /* even */
        *fn  = c;
        *fn1 = d;
    }
}

/* ---------- recursive divisor generation ---------- */
static void gen_divisors_rec(const uint64_t *pr, const int *ex,
                             size_t pf_cnt, uint64_t cur,
                             uint64_t *buf, size_t *num)
{
    if (pf_cnt == 0) {
        buf[(*num)++] = cur;
        return;
    }
    uint64_t p = pr[0];
    int e = ex[0];
    uint64_t val = 1;
    for (size_t i = 0; i <= (size_t)e; ++i) {
        gen_divisors_rec(pr + 1, ex + 1, pf_cnt - 1,
                         cur * val, buf, num);
        val *= p;
    }
}

/* ---------- Pisano period for a prime (p ≠ 5) ---------- */
static seq_t pisano_period_prime(uint64_t p)
{
    if (p == 2) return (seq_t)3;
    if (p == 3) return (seq_t)8;

    uint64_t bound;
    uint64_t p_mod5 = p % 5;
    if (p_mod5 == 1 || p_mod5 == 4)
        bound = p - 1;
    else
        bound = 2 * (p + 1);

    /* factor bound */
    uint64_t pf[32];
    int     exp[32];
    size_t  pf_cnt = 0;
    uint64_t tmp = bound;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t q = prime_list[i];
        if (tmp % q == 0) {
            int e = 0;
            while (tmp % q == 0) {
                tmp /= q;
                ++e;
            }
            pf[pf_cnt]   = q;
            exp[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        pf[pf_cnt]   = tmp;
        exp[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* generate all divisors of bound */
    uint64_t divs[256];
    size_t num_divs = 0;
    gen_divisors_rec(pf, exp, pf_cnt, 1, divs, &num_divs);

    /* sort divisors */
    for (size_t i = 0; i < num_divs; ++i)
        for (size_t j = i + 1; j < num_divs; ++j)
            if (divs[i] > divs[j]) {
                uint64_t t = divs[i];
                divs[i] = divs[j];
                divs[j] = t;
            }

    /* find minimal divisor giving one full period */
    for (size_t i = 0; i < num_divs; ++i) {
        uint64_t d = divs[i];
        uint64_t f, fp1;
        fib_mod(d, p, &f, &fp1);
        if (f == 0 && fp1 == 1)
            return (seq_t)d;
    }
    /* should never happen */
    return (seq_t)bound;
}

/* ---------- Pisano period cache ---------- */
static seq_t *pisano_cache = NULL;
static size_t pisano_cap  = 0;

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for n < 1 */
    ensure_primes(n);

    uint64_t p = prime_list[(size_t)(n - 1)];

    /* handle prime 5 */
    if (p == 5)
        return (seq_t)0;

    /* ensure pisano cache big enough */
    if (pisano_cap < prime_cnt) {
        seq_t *tmp = (seq_t *)realloc(pisano_cache, prime_cnt * sizeof(seq_t));
        pisano_cache = tmp;
        pisano_cap   = prime_cnt;
    }

    seq_t period = pisano_cache[(size_t)(n - 1)];
    if (period == 0) {
        period = pisano_period_prime(p);
        pisano_cache[(size_t)(n - 1)] = period;
    }

    uint64_t num;
    uint64_t p_mod5 = p % 5;
    if (p_mod5 == 1 || p_mod5 == 4)
        num = 2 * (p - 1);
    else
        num = 2 * (p + 1);

    seq_t result = (seq_t)(num / period);
    return result;
}
