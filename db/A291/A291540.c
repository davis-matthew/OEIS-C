/*
 * A291540  a(n) = PrimePi(n) * PrimePi(n^2) - PrimePi(n)^3, where PrimePi =
 *          A000720.
 * https://oeis.org/A291540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = PrimePi(n) * PrimePi(n^2) - PrimePi(n)^3
 * Range:     i8 n <= 11, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements the sequence A291540.  Uses an incremental odd‑only
 *  sieve.  The sieve is extended on demand; the prime list is
 *  kept sorted so that PrimePi(x) is obtained by binary search. */

#include <stdint.h>
#include <stdlib.h>

/* Bit manipulation helpers for the sieve array */
static inline int bit_is_set(const uint8_t *arr, size_t idx) {
    return (arr[idx >> 3] >> (idx & 7)) & 1;
}
static inline void bit_set(uint8_t *arr, size_t idx) {
    arr[idx >> 3] |= (1 << (idx & 7));
}

/* Static storage for sieve and prime list */
static uint8_t *sieve_buf   = NULL;   /* odd numbers starting at 3 */
static size_t   sieve_bits  = 0;      /* number of odd entries */
static idx_t    sieve_limit = 0;      /* last expanded bound */
static uint32_t *prime_list  = NULL;  /* sorted list of primes */
static size_t   prime_cnt   = 0;
static size_t   prime_cap   = 0;

/* Expand the sieve so that it contains all primes ≤ limit (inclusive). */
static void _expand_sieve(idx_t limit) {
    if (limit <= sieve_limit) return;

    /* Free previous data */
    if (sieve_buf) {
        free(sieve_buf);
        sieve_buf   = NULL;
        sieve_bits  = 0;
    }
    if (prime_list) {
        free(prime_list);
        prime_list   = NULL;
        prime_cnt    = 0;
        prime_cap    = 0;
    }

    /* Ensure we build at least up to 3 so that the prime 2 is included. */
    idx_t new_limit = (limit < 3) ? 3 : limit;

    /* Prepare sieve for odd numbers 3,5,7,… */
    if (new_limit >= 3) {
        sieve_bits = ((size_t)new_limit - 3) / 2 + 1;
        size_t sieve_bytes = (sieve_bits + 7) / 8;
        sieve_buf = (uint8_t *)calloc(sieve_bytes, 1);
        if (!sieve_buf) exit(EXIT_FAILURE);
    }

    /* Classic odd‑only sieve */
    for (size_t i = 0; i < sieve_bits; ++i) {
        if (bit_is_set(sieve_buf, i)) continue;
        uint32_t p = (uint32_t)(2 * i + 3);   /* candidate prime */
        uint64_t start = (uint64_t)p * (uint64_t)p;
        if (start > (uint64_t)new_limit) break;
        size_t j = ((size_t)start - 3) / 2;
        for (; j < sieve_bits; j += p)
            bit_set(sieve_buf, j);
    }

    /* Allocate prime list with a moderate initial capacity */
    size_t estimated = (new_limit > 20) ? (size_t)new_limit / 20 : 10;
    prime_list = (uint32_t *)malloc(estimated * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);
    prime_cap  = estimated;
    prime_cnt  = 0;

    /* Insert prime 2 if needed */
    if (new_limit >= 2) {
        prime_list[prime_cnt++] = 2;
        if (prime_cnt == prime_cap) {
            prime_cap *= 2;
            prime_list = (uint32_t *)realloc(prime_list,
                                            prime_cap * sizeof(uint32_t));
            if (!prime_list) exit(EXIT_FAILURE);
        }
    }

    /* Add odd primes from the sieve */
    for (size_t i = 0; i < sieve_bits; ++i) {
        if (!bit_is_set(sieve_buf, i)) {
            uint32_t p = (uint32_t)(2 * i + 3);
            if (p > (uint32_t)new_limit) break;
            prime_list[prime_cnt++] = p;
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint32_t *)realloc(prime_list,
                                                prime_cap * sizeof(uint32_t));
                if (!prime_list) exit(EXIT_FAILURE);
            }
        }
    }

    sieve_limit = new_limit;
}

/* Returns PrimePi(x) using the cached sieve and binary search. */
static seq_t _pi_of(idx_t x) {
    if (x < 2) return (seq_t)0;
    _expand_sieve(x);
    /* Upper‑bound binary search on prime_list */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if ((seq_t)prime_list[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)lo;
}

/* Sequence function */
seq_t a(idx_t n) {
    seq_t pi_n  = _pi_of(n);
    idx_t n_sq  = n * n;
    seq_t pi_sq = _pi_of(n_sq);
    seq_t part1 = pi_n * pi_sq;
    seq_t part2 = pi_n * pi_n * pi_n;
    return part1 - part2;
}
