/*
 * A392201  Positive k such that the quadratic Diophantine equation x^2 + y^2
 *          + z^2 = k * (x*y - x*z + y*z) has nontrivial integer solutions.
 * https://oeis.org/A392201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A331605(n) + 1
 * Range:     u8 n <= 32, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Positive k such that the square‑free part of (k‑2)*(k+1) has
   no prime factors ≡ 2 (mod 3).  This code generates the
   sequence and caches computed values. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime generation ---------- */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;

static void init_primes(void)
{
    static bool initialized = false;
    if (initialized) return;

    /* upper bound for sqrt(max k we will check) – plenty large */
    const uint64_t LIMIT = 1000000;          /* ~√(10^12) */
    bool *is_comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_comp) abort();

    for (uint64_t i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;

    /* count primes */
    prime_cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++prime_cnt;

    prime_list = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) prime_list[idx++] = i;

    free(is_comp);
    initialized = true;
}

/* factorise n, storing distinct primes and the parity of their exponents
   (true = odd, false = even). Returns the number of distinct primes. */
static size_t factor_parity(uint64_t n, uint64_t *pr, bool *par, size_t maxfac)
{
    if (n <= 1) return 0;
    size_t cnt = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= n; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0) {
            bool odd = false;
            while (n % p == 0) {
                n /= p;
                odd = !odd;
            }
            if (cnt >= maxfac) break;
            pr[cnt] = p;
            par[cnt] = odd;
            ++cnt;
        }
    }
    if (n > 1) {           /* remaining prime factor */
        if (cnt < maxfac) {
            pr[cnt] = n;
            par[cnt] = true;   /* exponent 1 → odd */
            ++cnt;
        }
    }
    return cnt;
}

/* test whether a given k satisfies the sequence condition */
static bool good_k(uint64_t k)
{
    if (k < 2) return false;          /* sequence starts at 2 */
    if (k == 2) return true;          /* (k-2)(k+1)=0 → squarefree part empty */

    init_primes();

    uint64_t a = k - 2;
    uint64_t b = k + 1;

    /* factor a and b, collecting prime parities */
    uint64_t pa[64], pb[64];
    bool   ea[64], eb[64];
    size_t ca = factor_parity(a, pa, ea, 64);
    size_t cb = factor_parity(b, pb, eb, 64);

    /* first, primes appearing in a */
    for (size_t ia = 0; ia < ca; ++ia) {
        uint64_t p = pa[ia];
        bool odd = ea[ia];
        /* see if p also appears in b */
        for (size_t ib = 0; ib < cb; ++ib) {
            if (pb[ib] == p) {
                odd ^= eb[ib];
                break;
            }
        }
        if (odd && (p % 3 == 2)) return false;
    }

    /* then primes that appear only in b */
    for (size_t ib = 0; ib < cb; ++ib) {
        uint64_t p = pb[ib];
        bool in_a = false;
        for (size_t ia = 0; ia < ca; ++ia)
            if (pa[ia] == p) { in_a = true; break; }
        if (!in_a && eb[ib] && (p % 3 == 2)) return false;
    }
    return true;
}

/* ---------- sequence cache ---------- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t last_k   = 1;           /* last integer inspected */

static void ensure_cache(size_t need)
{
    if (cache_len >= need) return;

    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    while (cache_len < need) {
        ++last_k;
        if (good_k(last_k)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[cache_len++] = (seq_t)last_k;
        }
    }
}

extern seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined below offset */
    ensure_cache((size_t)n);
    return cache[n - 1];
}
