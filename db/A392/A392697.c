/*
 * A392697  Numbers whose prime indices are the 0-prepended first sums of some
 *          finite sequence of distinct integers.
 * https://oeis.org/A392697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = nth natural number whose prime‑index multiset q satisfies
 *            that * the trimmed 0‑based partial alternating sums of q are all
 *            distinct
 * Range:     u8 n <= 174, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PRIME_LIMIT 2000000   /* enough for typical usage (n up to ~10000) */
#define MAX_FACTORS     2048      /* maximum number of prime factors to check for duplicates */

/* static state: primes list */
static uint32_t *prime_list   = NULL;
static size_t   prime_count   = 0;

/* forward declarations */
static void      init_primes(void);
static inline uint32_t prime_index(uint32_t p);
static bool      is_good(uint64_t n);
static void      generate_upto(idx_t n);

/* cache of sequence terms */
static seq_t   *seq_cache = NULL;
static size_t   seq_len   = 0;
static size_t   seq_cap   = 0;
static uint64_t next_candidate = 1;

/* --------------------------------------------------------------------------- */
/* Prime sieve: fill prime_list and prime_count */
static void init_primes(void)
{
    if (prime_list != NULL) return;   /* already initialized */

    size_t limit = MAX_PRIME_LIMIT;
    bool *is_comp = (bool*)malloc(limit + 1);
    memset(is_comp, 0, limit + 1);

    /* provisional array big enough for all primes up to limit */
    prime_list = (uint32_t*)malloc(limit * sizeof(uint32_t));
    size_t cnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            prime_list[cnt++] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }

    prime_count = cnt;
    prime_list   = (uint32_t*)realloc(prime_list, prime_count * sizeof(uint32_t));
    free(is_comp);
}

/* binary search to obtain the rank of a prime (1‑based) */
static inline uint32_t prime_index(uint32_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* p is guaranteed to be prime; lo points to it */
    return (uint32_t)(lo + 1);
}

/* checks whether n satisfies the alternating‑sum distinct property */
static bool is_good(uint64_t n)
{
    if (n == 1) return true;          /* empty multiset is acceptable */

    long long s_prev = 0;
    long long seen[MAX_FACTORS];
    size_t   seen_cnt = 0;

    uint64_t tmp = n;

    /* factor using the pre‑computed prime list */
    for (size_t idx = 0; idx < prime_count && (uint64_t)prime_list[idx] * prime_list[idx] <= tmp; ++idx) {
        uint32_t p = prime_list[idx];
        if (tmp % p == 0) {
            uint32_t primeIdx = (uint32_t)(idx + 1);   /* rank of p */
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            for (int e = 0; e < exp; ++e) {
                s_prev = -s_prev + primeIdx;
                for (size_t t = 0; t < seen_cnt; ++t)
                    if (seen[t] == s_prev) return false;
                seen[seen_cnt++] = s_prev;
            }
        }
    }

    if (tmp > 1) {   /* remaining prime factor */
        uint32_t p = (uint32_t)tmp;
        uint32_t primeIdx = prime_index(p);
        s_prev = -s_prev + primeIdx;
        for (size_t t = 0; t < seen_cnt; ++t)
            if (seen[t] == s_prev) return false;
        seen[seen_cnt++] = s_prev;
    }

    return true;
}

/* extend the cache up to position n (1‑based) */
static void generate_upto(idx_t n)
{
    if (seq_len >= (size_t)n) return;

    while (seq_len < (size_t)n) {
        if (is_good(next_candidate)) {
            seq_t val = (seq_t)next_candidate;
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 8;
                seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = val;
        }
        ++next_candidate;
    }
}

/* --------------------------------------------------------------------------- */
/* The required external function */
seq_t a(idx_t n)
{
    init_primes();          /* ensure primes are ready */
    if (n == 0) return (seq_t)0;   /* undefined for n=0; harmless */

    generate_upto(n);
    return seq_cache[n - 1];
}
