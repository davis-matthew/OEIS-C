/*
 * A295588  Numbers k such that Bernoulli number B_{k} has denominator 14322.
 * https://oeis.org/A295588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 89, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A295588: numbers k such that the denominator of the Bernoulli number B_k is 14322
 *  The denominator of B_k (for even k > 1) is the product of all primes p with p‑1 | k.
 *  Hence B_k has denominator 14322 = 2*3*7*11*31 ⟺ the set of primes {p : p‑1 | k}
 *  equals {2,3,7,11,31}.  
 *  This occurs exactly for those even k that are multiples of 30 and
 *  have no other divisor d > 1 such that d+1 is prime.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 30;      /* first multiple of 30 */

/* ------------------------------------------------------------------ */
/*  Simple prime sieve up to 5000 (more than enough for factoring
 *  numbers < 1e7).                                                    */
/* ------------------------------------------------------------------ */
#define PRIME_LIMIT 5000
static uint32_t primes[700];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    bool mark[PRIME_LIMIT + 1];
    memset(mark, 0, sizeof(mark));
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!mark[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if ((int64_t)i * i <= PRIME_LIMIT) {
                for (int j = i * i; j <= PRIME_LIMIT; j += i) mark[j] = true;
            }
        }
    }
    primes_ready = true;
}

/* ------------------------------------------------------------------ */
/*  Trial‑division factorisation (returns prime factors, exponents,
 *  and their count).                                                  */
/* ------------------------------------------------------------------ */
static void factor_idx(idx_t n, uint64_t *pf, uint8_t *exp, size_t *cnt)
{
    init_primes();
    idx_t tmp = n;
    size_t c = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= (uint64_t)tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            pf[c] = p;
            exp[c] = 0;
            while (tmp % p == 0) {
                exp[c]++; tmp /= p;
            }
            c++;
        }
    }
    if (tmp > 1) {
        pf[c] = (uint64_t)tmp;
        exp[c] = 1;
        c++;
    }
    *cnt = c;
}

/* ------------------------------------------------------------------ */
/*  64‑bit primality test by trial division (fast for n < 10⁷).        */
/* ------------------------------------------------------------------ */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t i = 5;
    uint64_t w = 2;
    while (i * i <= n) {
        if (n % i == 0) return false;
        i += w;
        w = 6 - w;
    }
    return true;
}

static bool allowed_prime(uint64_t p)
{
    return p == 3u || p == 7u || p == 11u || p == 31u;
}

/* ------------------------------------------------------------------ */
/*  Generate all divisors of a number from its factorisation.         */
/* ------------------------------------------------------------------ */
static void gen_divisors_rec(size_t idx, uint64_t cur,
    const uint64_t *pf, const uint8_t *exp, size_t cnt,
    uint64_t *out, size_t *pos)
{
    if (idx == cnt) {
        out[*pos] = cur;
        (*pos)++;
        return;
    }
    uint64_t val = cur;
    for (int e = 0; e <= exp[idx]; ++e) {
        gen_divisors_rec(idx + 1, val, pf, exp, cnt, out, pos);
        val *= pf[idx];
    }
}

/* ------------------------------------------------------------------ */
/*  Test whether a given candidate k has denominator 14322.            */
/* ------------------------------------------------------------------ */
static bool is_valid_k(idx_t k)
{
    if (k % 30 != 0) return false;       /* safety, should not happen */
    uint64_t pf[32];
    uint8_t exp[32];
    size_t cnt;
    factor_idx(k, pf, exp, &cnt);

    /* We only need a buffer sufficient for all divisors of numbers < 1e7. */
    static uint64_t divisors[2048];
    size_t pos = 0;
    gen_divisors_rec(0, 1, pf, exp, cnt, divisors, &pos);

    for (size_t i = 0; i < pos; ++i) {
        uint64_t d = divisors[i];
        if (d <= 1) continue;                 /* skip 1 */
        uint64_t p = d + 1;
        if (is_prime_uint64(p) && !allowed_prime(p))
            return false;                     /* extra prime found */
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Append a found value into the growing cache.                       */
/* ------------------------------------------------------------------ */
static void push_cache(idx_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;    /* allocation failed – unlikely in test harness */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t)val;
}

/* ------------------------------------------------------------------ */
/*  Public function required by the compiler.                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but harmless */

    while (cache_len < n) {
        if (is_valid_k(next_candidate))
            push_cache(next_candidate);
        next_candidate += 30;          /* only multiples of 30 are possible */
    }
    return cache[n - 1];
}
