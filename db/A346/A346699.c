/*
 * A346699  Sum of the odd bisection (odd-indexed parts) of the integer
 *          partition with Heinz number n.
 * https://oeis.org/A346699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = sum of odd‑indexed parts of the partition whose Heinz
 *            number is n
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static prime table */
static uint64_t *prime_list  = NULL;  /* ascending list of primes */
static size_t   prime_count = 0;      /* number of primes */
static idx_t    prime_limit = 0;      /* largest n up to which primes are stored */

/* Ensure we have all primes ≤ limit.  Simple incremental sieve. */
static void ensure_primes_upto(idx_t limit)
{
    if (prime_limit >= limit) return;

    /* free old table */
    free(prime_list);
    prime_list = NULL;
    prime_count = 0;

    /* sieve array */
    size_t sz = (size_t)(limit + 1);
    bool *is_prime = (bool *)calloc(sz, sizeof(bool));
    if (is_prime == NULL) return;          /* out of memory – abort */

    for (size_t i = 2; i < sz; ++i) is_prime[i] = true;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = false;
        }
    }

    /* count primes */
    size_t pc = 0;
    for (size_t i = 2; i < sz; ++i)
        if (is_prime[i]) ++pc;

    /* allocate array for primes */
    prime_list = (uint64_t *)malloc(pc * sizeof(uint64_t));
    if (prime_list == NULL) { free(is_prime); return; }

    /* store primes */
    size_t pos = 0;
    for (size_t i = 2; i < sz; ++i)
        if (is_prime[i]) prime_list[pos++] = (uint64_t)i;

    prime_count = pc;
    prime_limit = limit;
    free(is_prime);
}

/* Binary search: return 1‑based index of prime p in prime_list.  p is assumed prime and present. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count;          /* hi exclusive */
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo + 1;     /* 1‑based */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0 */

    /* We may need primes up to n (largest possible prime factor). */
    ensure_primes_upto((idx_t)n);

    /* Factor n into primes, collecting their indices (with multiplicity). */
    uint64_t m = (uint64_t)n;
    size_t  cap   = 64;                     /* sufficient for n ≤ 64‑bit */
    seq_t  *idx_arr = (seq_t *)malloc(cap * sizeof(seq_t));
    if (idx_arr == NULL) return 0;          /* memory error – fallback */

    size_t idx_cnt = 0;
    for (size_t i = 0; i < prime_count &&
         ((unsigned __int128)prime_list[i] * prime_list[i] <= m); ++i) {
        uint64_t p = prime_list[i];
        while (m % p == 0) {
            if (idx_cnt >= cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(idx_arr, cap * sizeof(seq_t));
                if (tmp == NULL) { free(idx_arr); return 0; }
                idx_arr = tmp;
            }
            idx_arr[idx_cnt++] = (seq_t)prime_index(p);
            m /= p;
        }
    }
    if (m > 1) {   /* remaining prime factor */
        if (idx_cnt >= cap) {
            cap *= 2;
            seq_t *tmp = (seq_t *)realloc(idx_arr, cap * sizeof(seq_t));
            if (tmp == NULL) { free(idx_arr); return 0; }
            idx_arr = tmp;
        }
        idx_arr[idx_cnt++] = (seq_t)prime_index(m);
    }

    if (idx_cnt == 0) { free(idx_arr); return 0; }   /* should not happen for n>1 */

    /* Sort indices in descending order (largest first).  Simple insertion sort. */
    for (size_t i = 1; i < idx_cnt; ++i) {
        seq_t key = idx_arr[i];
        size_t j = i;
        while (j > 0 && idx_arr[j - 1] < key) {
            idx_arr[j] = idx_arr[j - 1];
            --j;
        }
        idx_arr[j] = key;
    }

    /* Sum the parts at odd positions (1‑based: indices 0,2,4,...) */
    seq_t sum = 0;
    for (size_t i = 0; i < idx_cnt; i += 2)
        sum += idx_arr[i];

    free(idx_arr);
    return sum;
}
