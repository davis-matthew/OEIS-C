/*
 * A327520  Number of factorizations of the n-th stable number A316476(n) into
 *          stable numbers > 1.
 * https://oeis.org/A327520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   build_primes_upto, power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of factorizations of the n‑th stable number
   into stable numbers > 1. */

/*  The sequence A327520 requires
        1. generating all stable numbers in increasing order,
        2. for each stable number S, counting unordered multiplicative
           factorizations of S into stable numbers > 1.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ----------  utilities  ---------- */
#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/power.h"               /* seq_t cast from literal */

/* ----------  prime generation  ---------- */
#define PRIME_LIMIT 2000000            /* sufficient for first 10 000 stable numbers */

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_list != NULL) return;
    build_primes_upto(PRIME_LIMIT, &prime_list, &prime_count);
}

/* ----------  factorisation helper  ---------- */
static size_t factor_uint64(uint64_t n, uint64_t *primes_out, uint64_t *exps_out)
{
    size_t cnt = 0;
    if (n <= 1) return 0;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= n; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0) {
            primes_out[cnt] = p;
            exps_out[cnt] = 0;
            while (n % p == 0) {
                n /= p;
                ++exps_out[cnt];
            }
            ++cnt;
        }
    }
    if (n > 1) {                      /* remaining prime factor */
        primes_out[cnt] = n;
        exps_out[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* ----------  binary search for prime index (1‑based)  ---------- */
static size_t prime_index_of_uint64(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= p) lo = mid + 1;
        else hi = mid;
    }
    if (lo > 0 && prime_list[lo - 1] == p) return lo;   /* 1‑based index */
    return 0;
}

/* ----------  integer square root (floor)  ---------- */
static uint64_t int_sqrt(uint64_t x)
{
    uint64_t r = 0;
    for (uint64_t b = (uint64_t)1 << 31; b; b >>= 1) {
        uint64_t t = r + b;
        if (t * t <= x) r = t;
    }
    return r;
}

/* ----------  stability test  ---------- */
static bool is_stable(uint64_t n)
{
    if (n <= 1) return false;          /* stable numbers start from 2 */
    uint64_t pr[64], ex[64];
    size_t cnt = factor_uint64(n, pr, ex);
    if (cnt == 0) return false;        /* should not happen */

    uint64_t idx[64];
    for (size_t i = 0; i < cnt; ++i) {
        idx[i] = prime_index_of_uint64(pr[i]);   /* 1‑based index */
        if (idx[i] == 0) return false;          /* too large prime */
    }

    /* pairwise indivisibility */
    for (size_t i = 0; i < cnt; ++i) {
        if (idx[i] == 1 && cnt > 1) return false;  /* index 1 divides all others */
        for (size_t j = i + 1; j < cnt; ++j) {
            if (idx[i] <= idx[j] && idx[j] % idx[i] == 0) return false;
            if (idx[j] <= idx[i] && idx[i] % idx[j] == 0) return false;
        }
    }
    return true;
}

/* ----------  storage for stable numbers and their factorisation counts  ---------- */
static uint64_t *stable_nums = NULL;   /* stable numbers (uint64_t) */
static seq_t   *fact_counts = NULL;   /* corresponding counts (seq_t) */
static size_t stable_cap  = 0;
static size_t stable_cnt  = 0;
static uint64_t next_candidate = 2;

/* Ensure capacity for the stable arrays */
static void ensure_cap(size_t needed)
{
    if (stable_cap >= needed) return;
    size_t newcap = stable_cap ? stable_cap * 2 + 16 : 64;
    while (newcap < needed) newcap = newcap * 2 + 16;
    stable_nums  = (uint64_t *)realloc(stable_nums,  newcap * sizeof(uint64_t));
    fact_counts  = (seq_t *)realloc(fact_counts,  newcap * sizeof(seq_t));
    stable_cap   = newcap;
}

/* ----------  recursive counting of factorizations into stable numbers  ---------- */
static seq_t count_factorizations(uint64_t num, size_t min_idx)
{
    if (num == 1) return (seq_t)1;           /* empty product */
    seq_t total = 0;
    uint64_t limit = int_sqrt(num);
    for (size_t i = min_idx; i < stable_cnt; ++i) {
        uint64_t d = stable_nums[i];
        if (d > limit) break;
        if (num % d == 0) {
            total += count_factorizations(num / d, i);
        }
    }
    return total + (seq_t)1;                 /* trivial factorisation [num] */
}

/* ----------  expanding the cache to contain at least target stable numbers  ---------- */
static void grow_cache(idx_t target)
{
    if (stable_cnt >= (size_t)target) return;
    while (stable_cnt < (size_t)target) {
        if (is_stable(next_candidate)) {
            ensure_cap(stable_cnt + 1);
            stable_nums[stable_cnt] = next_candidate;
            fact_counts[stable_cnt] = count_factorizations(next_candidate, 0);
            ++stable_cnt;
        }
        ++next_candidate;
    }
}

/* ----------  public function required by the problem  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset is 1, below that unused */
    static bool initialized = false;
    if (!initialized) {
        init_primes();
        ensure_cap(64);
        initialized = true;
    }
    grow_cache(n);
    return fact_counts[(size_t)n - 1];
}
