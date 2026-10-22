/*
 * A352101  The numbers formed by removing the required prime factors to form
 *          the number from a set which is initially empty and that has primes
 *          added via the addition of the missing prime factors of numbers
 *          which cannot be created from those currently in the set. Start by
 *          trying to create the number 2.
 * https://oeis.org/A352101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352101: numbers obtained by the prime‑set construction
   algorithm described in the OEIS entry.  The function a(n)
   produces the nth term (offset 1) by simulating the process.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----------   prime list and counts   ------------------- */
static uint64_t *prime_list = NULL;   /* primes, sorted increasingly   */
static uint64_t *prime_count = NULL;  /* multiplicities for the primes */
static size_t prime_cnt   = 0;         /* number of primes stored       */
static size_t prime_cap   = 0;         /* capacity of the above arrays  */
static uint64_t prime_search_limit = 1;/* largest number whose primes
                                         *  have been generated       */

/* grow the prime arrays when needed */
static void prime_grow(size_t needed)
{
    if (needed <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    prime_list   = (uint64_t *)realloc(prime_list,   new_cap * sizeof(uint64_t));
    prime_count  = (uint64_t *)realloc(prime_count,  new_cap * sizeof(uint64_t));
    prime_cap    = new_cap;
}

/* add a new prime at the end of the list */
static void add_prime(uint64_t p)
{
    prime_grow(prime_cnt + 1);
    prime_list[prime_cnt] = p;
    prime_count[prime_cnt] = 0;
    ++prime_cnt;
}

/* binary search: return index of p in prime_list (p is guaranteed to be present) */
static size_t find_prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return mid;
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return prime_cnt; /* never reached for a present prime */
}

/* ----------   generate primes up to a given limit   ------------------- */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_search_limit) return;
    /* Generate primes from (prime_search_limit+1) … limit by trial division */
    uint64_t start = prime_search_limit + 1;
    if (start < 2) start = 2;
    for (uint64_t cand = start; cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt && (__int128)prime_list[i] * prime_list[i] <= cand; ++i) {
            if (cand % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) add_prime(cand);
    }
    prime_search_limit = limit;
}

/* ----------   simple integer square‑root (unsigned 64‑bit) --------------- */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* ----------   factorisation of n into prime powers   ------------------- */
static void factor_uint64(uint64_t n,
                          uint64_t fac_pr[32], uint32_t fac_ex[32], int *fac_cnt)
{
    *fac_cnt = 0;
    uint64_t x = n;
    uint64_t lim = isqrt_uint64(x);
    ensure_primes_upto(lim);

    /* factor using primes up to sqrt(x) */
    for (size_t i = 0; i < prime_cnt && (__int128)prime_list[i] * prime_list[i] <= x; ++i) {
        uint64_t p = prime_list[i];
        if (x % p == 0) {
            uint32_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            fac_pr[*fac_cnt] = p;
            fac_ex[*fac_cnt] = e;
            ++*fac_cnt;
        }
    }
    if (x > 1) {               /* remaining factor is prime */
        ensure_primes_upto(x);
        fac_pr[*fac_cnt] = x;
        fac_ex[*fac_cnt] = 1;
        ++*fac_cnt;
    }
}

/* ----------   storage of generated terms   ----------------------------- */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

static void terms_grow(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

static void add_term(idx_t m)
{
    terms_grow(terms_len + 1);
    terms[terms_len++] = (seq_t)m;
}

/* ----------   main simulation routine   -------------------------------- */
static idx_t current_m = 2;          /* next number to process */

/* generate terms until we have at least n of them */
static void generate_upto(idx_t n)
{
    while (terms_len < (size_t)n) {
        uint64_t fac_pr[32];
        uint32_t fac_ex[32];
        int fac_cnt;
        factor_uint64(current_m, fac_pr, fac_ex, &fac_cnt);

        /* check constructibility */
        bool can = true;
        for (int i = 0; i < fac_cnt; ++i) {
            size_t idx = find_prime_index(fac_pr[i]);
            if (prime_count[idx] < fac_ex[i]) { can = false; break; }
        }

        if (can) {
            /* use and remove needed primes */
            for (int i = 0; i < fac_cnt; ++i) {
                size_t idx = find_prime_index(fac_pr[i]);
                prime_count[idx] -= fac_ex[i];
            }
            add_term(current_m);
        } else {
            /* add missing prime multiplicities only */
            for (int i = 0; i < fac_cnt; ++i) {
                size_t idx = find_prime_index(fac_pr[i]);
                if (prime_count[idx] < fac_ex[i])
                    prime_count[idx] = fac_ex[i];
            }
        }
        ++current_m;
    }
}

/* ----------   public function   ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n==0, return 0 */
    generate_upto(n);
    return terms[n - 1];
}
