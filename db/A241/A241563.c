/*
 * A241563  Number of 3-element subsets of {1,...,n} whose sum has more than 2
 *          divisors.
 * https://oeis.org/A241563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the number of 3‑element subsets of {1,…,n}
   whose sum has more than two divisors.  The function is memoised
   and works for any n ≥ 1.  The implementation is O(n) per new
   value of n and uses a simple prime sieve. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ==== Prime sieve and list ==== */
static bool   *prime_sieve  = NULL;   /* prime_sieve[i] == true iff i is prime   */
static idx_t *prime_list   = NULL;   /* array of primes up to current limit     */
static size_t prime_count  = 0;        /* number of primes                        */
static idx_t  prime_limit  = 0;        /* largest number sieved so far            */

/* Build/extend a prime sieve up to 'limit' (inclusive). */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    size_t sz = (size_t)limit + 1;
    bool *sieve = (bool *)calloc(sz, sizeof(bool));
    if (!sieve) abort();

    for (idx_t i = 2; i <= limit; ++i) sieve[i] = true;
    for (idx_t p = 2; p * p <= limit; ++p)
        if (sieve[p])
            for (idx_t j = p * p; j <= limit; j += p) sieve[j] = false;

    /* Count primes and build list */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    idx_t *list = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!list) abort();

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i]) list[pos++] = i;

    free(prime_sieve);
    free(prime_list);
    prime_sieve = sieve;
    prime_list  = list;
    prime_count = cnt;
    prime_limit = limit;
}

/* ==== Cached sequence values ==== */
static seq_t  *a_cache   = NULL;   /* a(n) values, 1‑based, a_cache[0] unused   */
static seq_t  *prime_cnt = NULL;   /* cumulative count of triples with prime sum */
static uint32_t *pair_cnt = NULL;  /* pair sums i+j for i<j, indexed by sum     */
static size_t    pair_size = 0;     /* current allocated size for pair_cnt       */
static idx_t     current_n = 0;     /* highest n for which a_cache is valid      */

/* Extends cached data to cover at least n. */
static void extend_to(idx_t n)
{
    if (n <= current_n) return;

    /* Ensure primes up to 3*n. */
    ensure_primes(3 * n);

    /* Resize a_cache and prime_cnt arrays. */
    size_t new_len = (size_t)n + 1;          /* indices 0..n  */
    a_cache   = (seq_t *)realloc(a_cache,   new_len * sizeof(seq_t));
    prime_cnt = (seq_t *)realloc(prime_cnt, new_len * sizeof(seq_t));
    if (!a_cache || !prime_cnt) abort();
    /* Initialise new entries to 0 for safety */
    for (size_t i = current_n + 1; i <= n; ++i) {
        a_cache[i]   = 0;
        prime_cnt[i] = 0;
    }
    a_cache[0]   = 0;
    prime_cnt[0] = 0;
    a_cache[1]   = 0;
    prime_cnt[1] = 0;
    if (n >= 2) {
        a_cache[2]   = 0;
        prime_cnt[2] = 0;
    }

    /* Resize pair_cnt array (max sum = 2*n). */
    size_t new_pair_size = (size_t)2 * n + 1;
    uint32_t *new_pair = NULL;
    if (pair_cnt) {
        new_pair = (uint32_t *)realloc(pair_cnt, new_pair_size * sizeof(uint32_t));
        if (!new_pair) abort();
        /* Zero newly allocated tail. */
        memset(new_pair + pair_size, 0, (new_pair_size - pair_size) * sizeof(uint32_t));
    } else {
        new_pair = (uint32_t *)calloc(new_pair_size, sizeof(uint32_t));
        if (!new_pair) abort();
    }
    pair_cnt  = new_pair;
    pair_size = new_pair_size;

    /* Main incremental computation. */
    for (idx_t k = current_n + 1; k <= n; ++k) {
        if (k < 3) {
            /* No triples yet. */
            a_cache[k]    = 0;
            prime_cnt[k]  = 0;
        } else {
            /* Count new triples with maximum element = k and prime sum. */
            uint32_t add = 0;
            idx_t p_start = k + 3;          /* smallest possible prime sum for new triples */
            idx_t p_end   = 3 * k - 3;      /* largest possible prime sum for new triples */

            for (size_t pi = 0; pi < prime_count; ++pi) {
                idx_t p = prime_list[pi];
                if (p < p_start) continue;
                if (p > p_end) break;
                idx_t s = p - k;            /* sum of the other two numbers (i + j) */
                if (s >= 3 && s <= 2 * k - 3) add += pair_cnt[s];
            }

            prime_cnt[k] = prime_cnt[k - 1] + (seq_t)add;

            seq_t N = (seq_t)k;
            seq_t total = N * (N - 1) * (N - 2) / 6;
            a_cache[k] = total - prime_cnt[k];
        }

        /* Update pair sums to include pairs that involve the new element k. */
        for (idx_t i = 1; i < k; ++i) {
            ++pair_cnt[i + k];
        }
    }

    current_n = n;
}

/* ==== Public API ==== */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined for n=0; return 0 for safety. */
    extend_to(n);
    return a_cache[n];
}
