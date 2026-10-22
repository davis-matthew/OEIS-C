/*
 * A318712  Numbers k for which a riffle shuffle permutation with two or more
 *          packets permutes all except the first and the last of the k cards.
 * https://oeis.org/A318712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 4021, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A318712 – Numbers k for which a riffle shuffle permutation with two or more packets permutes all except the first and the last of the k cards.
   Computed by searching primes p such that p+1 has a divisor m (2 ≤ m ≤ (p+1)/2) that is a primitive root modulo p. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Modular exponentiation ---------- */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1) {
            __uint128_t t = (__uint128_t)result * b;
            result = (uint64_t)(t % mod);
        }
        exp >>= 1;
        if (exp == 0) break;
        __uint128_t t = (__uint128_t)b * b;
        b = (uint64_t)(t % mod);
    }
    return result;
}

/* ---------- Factorisation utilities ---------- */
static void prime_factors(uint64_t n, const uint64_t *primes, size_t primes_cnt,
                          uint64_t *out, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) {
            out[(*cnt)++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) {
        out[(*cnt)++] = n;
    }
}

static void generate_divisors(uint64_t n, const uint64_t *primes, size_t primes_cnt,
                              uint64_t *out, size_t *cnt)
{
    uint64_t factors[64], exps[64], fc = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            factors[fc] = p;
            exps[fc] = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exps[fc];
            }
            ++fc;
        }
    }
    if (tmp > 1) {
        factors[fc] = tmp;
        exps[fc] = 1;
        ++fc;
    }

    /* iterative multiplication of divisor combinations */
    size_t sz = 1;
    out[0] = 1;
    for (size_t i = 0; i < fc; ++i) {
        uint64_t p = factors[i];
        uint64_t mult[64];
        mult[0] = 1;
        for (uint64_t e = 1; e <= exps[i]; ++e) {
            mult[e] = mult[e-1] * p;
        }
        size_t old_sz = sz;
        for (size_t j = 0; j < old_sz; ++j) {
            uint64_t base = out[j];
            for (uint64_t e = 1; e <= exps[i]; ++e) {
                out[sz++] = base * mult[e];
            }
        }
    }
    *cnt = sz;
}

/* ---------- Primitive root test ---------- */
static bool is_primitive_root(uint64_t m, uint64_t p,
                              const uint64_t *prime_divs, size_t div_cnt)
{
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t q = prime_divs[i];
        if (powmod_uint64(m, (p - 1) / q, p) == 1) {
            return false;
        }
    }
    return true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* cache of computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* quick hit */
    if (n <= cache_len) return cache[n - 1];

    /* ---------- prime sieve (once) ---------- */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt    = 0;
    static size_t prime_next   = 0;   /* next prime index to test */

    if (prime_list == NULL) {
        const size_t LIMIT = 2000000;   /* sufficient for many terms */
        bool *composite = (bool*)calloc(LIMIT + 1, 1);
        prime_list = (uint64_t*)malloc(sizeof(uint64_t) * (LIMIT / 10));
        for (size_t i = 2; i <= LIMIT; ++i) {
            if (!composite[i]) {
                prime_list[prime_cnt++] = i;
                if (prime_cnt >= LIMIT / 10) {
                    prime_list = (uint64_t*)realloc(prime_list,
                     sizeof(uint64_t) * (prime_cnt + LIMIT / 10));
                }
                if (i * i <= LIMIT) {
                    for (size_t j = i * i; j <= LIMIT; j += i)
                        composite[j] = true;
                }
            }
        }
        free(composite);
    }

    /* ---------- grow cache until desired index ---------- */
    while (cache_len < n) {
        /* iterate primes from where we left off */
        for (size_t pi = prime_next; pi < prime_cnt; ++pi) {
            uint64_t p = prime_list[pi];
            if (p < 3) { prime_next = pi + 1; continue; }   /* skip p=2 */

            uint64_t k = p + 1;

            /* prime factors of p-1 */
            uint64_t pf[64], pfcnt = 0;
            prime_factors(p - 1, prime_list, prime_cnt, pf, &pfcnt);

            /* all divisors of k */
            uint64_t divs[16384], divcnt = 0;
            generate_divisors(k, prime_list, prime_cnt, divs, &divcnt);

            /* search a suitable m */
            bool found = false;
            for (size_t di = 0; di < divcnt && !found; ++di) {
                uint64_t m = divs[di];
                if (m < 2 || m > k / 2) continue;
                if (is_primitive_root(m, p, pf, pfcnt)) {
                    /* store k */
                    if (cache_len >= cache_cap) {
                        cache_cap = cache_cap ? cache_cap * 2 : 64;
                        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_len++] = (seq_t)k;
                    found = true;
                }
            }
            prime_next = pi + 1;   /* advance past this prime for next term */
            if (found) break;
        }
    }
    return cache[n - 1];
}
