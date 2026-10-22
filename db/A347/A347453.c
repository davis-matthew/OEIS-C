/*
 * A347453  Heinz numbers of odd-length integer partitions with integer
 *          alternating (or reverse-alternating) product.
 * https://oeis.org/A347453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer whose prime‑index multiset has odd
 *            length and whose integer alternating product is an integer
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/* ---------- Global tables for primes and smallest prime factor -------- */
#define SIEVE_LIMIT      10000000          /* primes up to 10^7         */
#define MAX_PRIMES       700000            /* > π(10^7) = 664963       */

static uint32_t spf[SIEVE_LIMIT + 1];      /* smallest prime factor       */
static uint32_t primes[MAX_PRIMES];       /* list of primes in order    */
static size_t   prime_count = 0;          /* number of primes found      */

/* --------------------------------------------------------------------- */
/* ---------- Prime index lookup --------------------------------------- */
/* Return 1‑based index of prime p in the primes array, or 0 if not found. */
static inline uint32_t get_prime_index(uint32_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p) return (uint32_t)(mid + 1);   /* 1‑based */
        if (primes[mid] < p) lo = mid + 1; else hi = mid;
    }
    return 0;
}

/* --------------------------------------------------------------------- */
/* ---------- Build sieve, spf, and prime list ------------------------- */
static bool sieve_built = false;

static void build_sieve(void)
{
    /* simple sieve for primes and smallest prime factor */
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) spf[i] = 0;
    spf[0] = spf[1] = 1;

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!spf[i]) {               /* i is prime */
            spf[i] = (uint32_t)i;
            if (prime_count < MAX_PRIMES) primes[prime_count++] = (uint32_t)i;
            if (i > SIEVE_LIMIT / i) continue; /* avoid overflow */
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                if (!spf[j]) spf[j] = (uint32_t)i;
        }
    }
    sieve_built = true;
}

/* --------------------------------------------------------------------- */
/* ---------- Check whether integer N satisfies the OEIS condition ----- */
static bool good(seq_t N)
{
    /* 1. Factor N into primes (|index|) */
    uint64_t n = (uint64_t)N;
    /* Temporary storage for prime factors of N */
    uint32_t pf_pr[32];
    uint8_t  pf_exp[32];
    size_t   pf_cnt = 0;

    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            uint8_t e = 0;
            do { n /= p; ++e; } while (n % p == 0);
            pf_pr[pf_cnt] = p;
            pf_exp[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (n > 1) {                     /* remaining prime factor */
        if (pf_cnt >= 32) return false;   /* overflow protection */
        pf_pr[pf_cnt] = (uint32_t)n;
        pf_exp[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* 2. Build multiset of prime indices (with multiplicity) */
    uint32_t idx_list[64];
    size_t   idx_cnt = 0;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint32_t idx = get_prime_index(pf_pr[i]);
        if (idx == 0) return false;        /* prime beyond sieve -> discard */
        for (uint8_t e = 0; e < pf_exp[i]; ++e) {
            if (idx_cnt >= 64) return false;
            idx_list[idx_cnt++] = idx;
        }
    }

    /* 3. Must have odd length */
    if ((idx_cnt & 1) == 0) return false;

    /* 4. Sort indices descending (small array, insertion sort) */
    for (size_t i = 1; i < idx_cnt; ++i) {
        uint32_t key = idx_list[i];
        size_t j = i;
        while (j > 0 && idx_list[j - 1] < key) {
            idx_list[j] = idx_list[j - 1];
            --j;
        }
        idx_list[j] = key;
    }

    /* 5. Accumulate prime exponents from numerator and denominator */
    uint32_t mp_pr[200];
    int32_t  mp_num[200];
    int32_t  mp_den[200];
    size_t   mp_cnt = 0;

    for (size_t pos = 0; pos < idx_cnt; ++pos) {
        uint32_t val = idx_list[pos];
        bool numer = (pos % 2 == 0);  /* pos 0 => numerator */
        /* factor val using spf */
        while (val > 1) {
            uint32_t p = spf[val];
            int32_t e = 0;
            while (val % p == 0) { val /= p; ++e; }
            /* locate prime in mp arrays */
            size_t k = 0;
            while (k < mp_cnt && mp_pr[k] != p) ++k;
            if (k == mp_cnt) {
                mp_pr[mp_cnt] = p;
                mp_num[mp_cnt] = 0;
                mp_den[mp_cnt] = 0;
                ++mp_cnt;
            }
            if (numer) mp_num[k] += e; else mp_den[k] += e;
        }
    }

    /* 6. Check denominator exponents <= numerator exponents */
    for (size_t k = 0; k < mp_cnt; ++k)
        if (mp_den[k] > mp_num[k]) return false;
    return true;
}

/* --------------------------------------------------------------------- */
/* ---------- Sequence function with memoization ----------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 2;   /* start search from 2 */

seq_t a(idx_t n)
{
    if (!sieve_built) build_sieve();

    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_candidate = 2;
    }

    while (cache_len < n) {
        /* Expand cache capacity if necessary */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        if (good((seq_t)next_candidate))
            cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return cache[n - 1];
}
