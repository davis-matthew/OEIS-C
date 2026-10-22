/*
 * A363134  Positive integers whose multiset of prime indices satisfies:
 *          (length) = 2*(minimum).
 * https://oeis.org/A363134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates A363134 by iterating through the natural numbers
   and testing each for the condition that the total count of
   prime‐indices equals twice the minimum prime‑index. */

/* USES OF THE HELPER LIBRARY ARE PROHIBITED; TO IMPLEMENT
   the sequence we only need standard integer arithmetic and
   dynamic memory allocation. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Prime sieve (one‑time, up to a fixed limit sufficient for n ≤ 10000) */
/* ------------------------------------------------------------------ */
static uint32_t *primes = NULL;   /* store all primes up to LIMIT */
static size_t     prime_cnt = 0;  /* number of primes stored */

/* Estimate: 10000th term is below 2·p10000 ≈ 2·104729 ≈ 2.1·10⁵.
   A sieve limit of 5 000 000 comfortably exceeds this value. */
#define PRIME_SIEVE_LIMIT 5000000

static void build_primes(void)
{
    if (prime_cnt > 0)
        return;                     /* already built */

    char *sieve = (char*)malloc((PRIME_SIEVE_LIMIT + 1) * sizeof(char));
    if (!sieve)
        exit(EXIT_FAILURE);
    memset(sieve, 0, PRIME_SIEVE_LIMIT + 1);

    for (uint32_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                sieve[j] = 1;
        }
    }

    /* count primes first to know array size */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!sieve[i])
            ++cnt;

    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!primes)
        exit(EXIT_FAILURE);

    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!sieve[i])
            primes[idx++] = i;

    prime_cnt = cnt;
    free(sieve);
}

/* ------------------------------------------------------------------ */
/*  Check whether a given number satisfies the sequence condition     */
/* ------------------------------------------------------------------ */
static bool is_good(uint64_t n)
{
    if (n < 4)               /* 1,2,3 are not in the sequence */
        return false;

    build_primes();          /* ensure primes are ready */

    uint64_t rem = n;
    size_t total = 0;
    uint32_t min_idx = UINT32_MAX;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > rem)
            break;
        if (rem % p == 0) {
            uint32_t idx = (uint32_t)(i + 1); /* 1‑based index of p */
            if (idx < min_idx)
                min_idx = idx;
            while (rem % p == 0) {
                ++total;
                rem /= p;
            }
        }
    }

    if (rem > 1) {           /* remaining factor is a prime > sqrt(n) */
        /* binary search for its index */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (primes[mid] < rem)
                lo = mid + 1;
            else
                hi = mid;
        }
        uint32_t idx = (uint32_t)(lo + 1);
        if (idx < min_idx)
            min_idx = idx;
        ++total;
    }

    return total == 2 * min_idx;
}

/* ------------------------------------------------------------------ */
/*  Sequence cache and generator                                         */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;   /* array of found terms */
static size_t seq_len   = 0;      /* number of terms stored */
static size_t seq_cap   = 0;      /* current capacity */

/* Ensure that seq_cache contains at least needed terms */
static void ensure_terms(size_t needed)
{
    static uint64_t candidate = 4; /* start from the first possible term */

    if (seq_len >= needed)
        return;

    while (seq_len < needed) {
        if (is_good(candidate)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap << 1 : 16;
                seq_t *new_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!new_cache)
                    exit(EXIT_FAILURE);
                seq_cache = new_cache;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)candidate;  /* cast to seq_t */
        }
        ++candidate;
    }
}

/* ------------------------------------------------------------------ */
/*  Public function required by the build system                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* 0 is not a term; offset is 1 */

    ensure_terms((size_t)n);
    return seq_cache[(size_t)n - 1];
}
