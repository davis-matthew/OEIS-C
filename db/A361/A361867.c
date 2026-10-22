/*
 * A361867  Positive integers > 1 whose prime indices satisfy (maximum) >
 *          2*(median).
 * https://oeis.org/A361867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer >1 whose multiset of * prime
 *            indices satisfies (maximum) > 2*(median). * * The implementation
 *            uses a simple sieve and trial division to * factor numbers,
 *            builds the multiset of prime indices, * and checks the
 *            condition. Results are cached so that calls * to a(idx_...
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime sieve utilities ---------- */

static uint64_t *prime_array   = NULL;   /* sorted list of primes */
static size_t   prime_cnt     = 0;       /* number of primes stored   */
static size_t   prime_limit   = 0;       /* largest prime currently stored */

/* Build a prime list up to 'limit' (inclusive).  The list is rebuilt
 * every time 'limit' increases, but for the modest limits needed here
 * this is inexpensive. */
static void ensure_primes_upto(size_t limit)
{
    if (limit <= prime_limit) return;   /* already sufficient */

    free(prime_array);
    prime_array = NULL;
    prime_cnt   = 0;
    prime_limit = limit;

    bool *sieve = (bool *)calloc(limit + 1, 1);
    if (!sieve) exit(1);                /* out‑of‑memory */

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i) sieve[j] = true;
        }
    }

    for (size_t i = 2; i <= limit; ++i) if (!sieve[i]) ++prime_cnt;
    if (prime_cnt == 0) return;

    prime_array = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_array) { free(sieve); exit(1); }

    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!sieve[i]) prime_array[pos++] = (uint64_t)i;

    free(sieve);
}

/* Return the index (1‑based) of a given prime in prime_array.
 * The prime must be present in the current list. */
static size_t prime_index(uint64_t prime)
{
    size_t low = 0, high = prime_cnt;
    while (low < high) {
        size_t mid = (low + high) >> 1;
        if (prime_array[mid] < prime) low = mid + 1;
        else high = mid;
    }
    return low;      /* prime_array[low] == prime */
}

/* ---------- Condition check ---------- */

typedef struct { idx_t idx; unsigned int exp; } factor_t;

static bool satisfies(uint64_t x)
{
    if (x <= 1) return false;

    /* Ensure we can factor x */
    if (x > prime_limit) ensure_primes_upto((size_t)(x * 2));

    factor_t fac[16];
    int fac_cnt = 0;
    uint64_t n = x;

    for (size_t i = 0; i < prime_cnt && prime_array[i] * prime_array[i] <= n; ++i) {
        uint64_t p = prime_array[i];
        if (n % p == 0) {
            unsigned int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            if (fac_cnt < 16)
                fac[fac_cnt++] = (factor_t){ (idx_t)(i + 1), e };
        }
    }
    if (n > 1) {                       /* remaining prime factor */
        size_t pos = prime_index(n);
        if (fac_cnt < 16)
            fac[fac_cnt++] = (factor_t){ (idx_t)(pos + 1), 1 };
    }

    /* Build the multiset of prime indices */
    size_t k = 0;
    for (int i = 0; i < fac_cnt; ++i) k += fac[i].exp;
    if (k == 0) return false;          /* should not happen for x>1 */

    idx_t idx_arr[64];
    size_t idx_len = 0;
    for (int i = 0; i < fac_cnt; ++i) {
        for (unsigned int t = 0; t < fac[i].exp; ++t)
            idx_arr[idx_len++] = fac[i].idx;
    }

    idx_t max_idx = idx_arr[idx_len - 1];

    if (k & 1) {                       /* odd number of indices */
        idx_t median = idx_arr[(k - 1) / 2];
        return max_idx > 2 * median;
    } else {                           /* even number of indices */
        idx_t left  = idx_arr[k / 2 - 1];
        idx_t right = idx_arr[k / 2];
        return max_idx > left + right; /* 2*median = left + right */
    }
}

/* ---------- Sequence cache ---------- */

static seq_t *seq_cache   = NULL;
static size_t seq_len     = 0;
static size_t seq_cap     = 0;
static uint64_t next_candidate = 2;   /* start testing from 2 */

/* Ensure that at least 'n' terms are cached. */
static void ensure_cache(idx_t n)
{
    while (seq_len < n) {
        /* Grow prime list if necessary */
        if (next_candidate > prime_limit)
            ensure_primes_upto((size_t)((next_candidate > SIZE_MAX/2) ?
                                        SIZE_MAX : next_candidate * 2));

        if (satisfies(next_candidate)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                seq_cache = tmp;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* ---------- Public API ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */
    ensure_cache(n);
    return seq_cache[n - 1];
}
