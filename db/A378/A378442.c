/*
 * A378442  Characteristic function of stable numbers: a(n) = 1 if the
 *          distinct prime indices of n are pairwise indivisible, otherwise 0.
 * https://oeis.org/A378442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if distinct prime indices of n are pairwise
 *            indivisible, otherwise 0
 * Range:     u8 n <= 8944, u16 n <= 8944, u32 n <= 8944, u64 n <= 8944,
 *            u128 n <= 8944, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *prime_list = NULL;   /* primes up to prime_limit */
static size_t     prime_count = 0;    /* number of primes stored */
static idx_t      prime_limit = 0;    /* current upper bound of primes generated */

/* Ensure that all primes up to 'limit' are stored in prime_list. */
static void ensure_prime_limit(idx_t limit)
{
    if (prime_list == NULL || prime_limit < limit) {
        idx_t new_limit = limit;
        if (new_limit < 2) new_limit = 2;

        /* Allocate sieve array */
        size_t sieve_size = (size_t)(new_limit + 1);
        char *is_composite = (char *)malloc(sieve_size);
        if (!is_composite) return;
        memset(is_composite, 0, sieve_size);

        /* Estimate initial capacity for prime list */
        size_t capacity = new_limit / 2 + 10;
        uint64_t *new_list = (uint64_t *)malloc(capacity * sizeof(uint64_t));
        if (!new_list) { free(is_composite); return; }

        size_t found = 0;
        for (idx_t num = 2; num <= new_limit; ++num) {
            if (!is_composite[num]) {
                if (found >= capacity) {
                    capacity <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(new_list, capacity * sizeof(uint64_t));
                    if (!tmp) { break; }          /* allocation failure – keep old data */
                    new_list = tmp;
                }
                new_list[found++] = (uint64_t)num;
                if (num * num <= new_limit) {
                    for (idx_t mult = num * num; mult <= new_limit; mult += num)
                        is_composite[mult] = 1;
                }
            }
        }
        free(is_composite);

        /* Replace old prime list */
        uint64_t *old_list = prime_list;
        prime_list  = new_list;
        prime_count = found;
        prime_limit = new_limit;
        free(old_list);
    }
}

/* Binary search: return 1‑based index of prime 'p' in prime_list. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return mid + 1;   /* 1‑based */
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return 0;   /* should not happen for valid input */
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;          /* no prime factors → vacuously true */

    ensure_prime_limit(n);

    /* Factorise n, keeping distinct primes. */
    uint64_t temp = (uint64_t)n;
    uint64_t factors[64];
    size_t   fcnt = 0;

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            factors[fcnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {                        /* remaining prime > sqrt(n) */
        factors[fcnt++] = temp;
    }

    /* Compute indices of the prime factors. */
    idx_t indices[64];
    for (size_t i = 0; i < fcnt; ++i) {
        indices[i] = (idx_t)prime_index(factors[i]);   /* guaranteed to exist */
    }

    /* Check pairwise indivisibility of indices. */
    for (size_t i = 0; i < fcnt; ++i) {
        for (size_t j = i + 1; j < fcnt; ++j) {
            idx_t ui = indices[i];
            idx_t uj = indices[j];
            if (ui % uj == 0 || uj % ui == 0) return (seq_t)0;
        }
    }

    return (seq_t)1;
}
