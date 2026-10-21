/*
 * A249300  Composite numbers whose concatenation of their aliquot parts, in
 *          ascending order, is a palindrome.
 * https://oeis.org/A249300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 59, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A249300
 *
 * Sequence: composite numbers whose concatenation of all proper divisors
 * (aliquot parts) in ascending order is a palindrome.
 *
 * A(n) is not given by a known simple formula; the routine searches
 * composite integers, factors them, builds the concatenated decimal
 * representation of the proper divisors, tests for palindrome, and
 * caches all found terms so that any order of requests is supported.
 *
 * The implementation uses a 64‑bit Miller–Rabin primality test and a
 * simple sieve of Eratosthenes for trial division.  All arithmetic
 * is performed with unsigned 64‑bit integers; the final value is cast
 * to seq_t before returning.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- helper for qsort: compare two uint64_t values ---------- */
static inline int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* ---------- modular arithmetic for 64‑bit integers ---------- */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* ---------- deterministic Miller–Rabin for 64‑bit integers ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    if (n % 5 == 0) return n == 5;
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- small prime sieve (up to 1 000 000) ---------- */
static uint64_t *prime_table = NULL;
static size_t prime_count  = 0;

static void init_primes(void)
{
    if (prime_table != NULL) return;
    const size_t LIMIT = 1000000;
    bool *is_comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_comp) return;
    prime_table = (uint64_t *)malloc(78500 * sizeof(uint64_t));
    prime_count = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            prime_table[prime_count++] = i;
            if (i <= LIMIT / i) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

/* ---------- factorization by trial division using the sieve ---------- */
static void factor_uint64(uint64_t n,
                          uint64_t *factors, int *exps, size_t *nf)
{
    *nf = 0;
    if (n < 2) return;
    for (size_t i = 0; i < prime_count && prime_table[i] * prime_table[i] <= n; ++i) {
        if (n % prime_table[i] == 0) {
            int e = 0;
            while (n % prime_table[i] == 0) {
                n /= prime_table[i];
                ++e;
            }
            factors[*nf] = prime_table[i];
            exps[*nf]    = e;
            ++(*nf);
        }
    }
    if (n > 1) {
        factors[*nf] = n;
        exps[*nf]    = 1;
        ++(*nf);
    }
}

/* ---------- generate all divisors from prime factorisation ---------- */
static void gen_divisors_rec(size_t idx, uint64_t cur,
                             const uint64_t *factors, const int *exps,
                             size_t nf, uint64_t *out, size_t *cnt)
{
    if (idx == nf) {
        out[(*cnt)++] = cur;
        return;
    }
    uint64_t p = factors[idx];
    uint64_t val = cur;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divisors_rec(idx + 1, val, factors, exps, nf, out, cnt);
        val *= p;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* static cache for previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t candidate = 4;          /* first composite to test */

    /* one‑time prime table construction */
    init_primes();

    /* initialize cache on first call */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* search until we have the n‑th term */
    while (cache_len < (size_t)n) {
        /* find next composite */
        while (true) {
            if (candidate <= 2) { candidate = 3; continue; }
            if (is_prime_uint64(candidate)) { ++candidate; continue; }
            break;
        }

        /* factorize candidate */
        uint64_t factors[16];
        int       exps[16];
        size_t    nf;
        factor_uint64(candidate, factors, exps, &nf);

        /* generate all divisors */
        uint64_t divs[8192];
        size_t div_cnt = 0;
        gen_divisors_rec(0, 1, factors, exps, nf, divs, &div_cnt);
        qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

        /* build concatenated decimal string of proper divisors */
        char buf[200000];
        size_t len = 0;
        for (size_t i = 0; i < div_cnt; ++i) {
            if (divs[i] == candidate) continue;      /* skip the number itself */
            uint64_t x = divs[i];
            char tmp[32];
            int pos = 0;
            do {
                tmp[pos++] = (char)('0' + (x % 10));
                x /= 10;
            } while (x);
            for (int j = pos - 1; j >= 0; --j)
                buf[len++] = tmp[j];
        }

        /* check palindrome */
        bool pal = true;
        for (size_t i = 0, j = len - 1; i < j; ++i, --j) {
            if (buf[i] != buf[j]) { pal = false; break; }
        }

        if (pal) {
            /* store term in cache */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)candidate;
        }
        ++candidate;
    }

    return cache[n - 1];
}
