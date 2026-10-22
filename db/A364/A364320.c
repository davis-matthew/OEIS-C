/*
 * A364320  Prime numbers that are the exact average of eight consecutive odd
 *          semiprimes.
 * https://oeis.org/A364320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 44, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   _smallest_prime_factor, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A364320: primes that are the exact average of eight consecutive odd semiprimes.
   Computed by scanning odd integers, testing odd semiprimes, and maintaining a sliding
   window of the last eight. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime_big.h"

/* Cache of already found terms */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Next odd integer to test for being an odd semiprime */
static idx_t cur_num = 3;

/* Sliding window of the last 8 odd semiprimes */
static uint64_t win_arr[8] = {0};
static size_t win_cnt = 0;
static seq_t win_sum = 0;

/* Test whether n is an odd semiprime (product of two odd primes, not necessarily distinct). */
static inline bool is_odd_semiprime(uint64_t n)
{
    if ((n & 1ULL) == 0)                /* even */
        return false;
    if (is_prime_big((seq_t)n))         /* n itself is prime */
        return false;

    uint64_t p = _smallest_prime_factor(n);
    if (p == 0)                         /* safety guard */
        return false;

    uint64_t q = n / p;
    if ((q & 1ULL) == 0)                /* q even => product not odd semiprime */
        return false;

    return is_prime_big((seq_t)q);      /* both factors odd primes? */
}

/* Append a new term to the cache, extending it as needed. */
static void append_term(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for offset 0 */

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        /* Find the next odd semiprime. */
        uint64_t cand;
        for (;;) {
            cand = cur_num;
            cur_num += 2;
            if (is_odd_semiprime(cand))
                break;
        }

        /* Update sliding window of the last 8 odd semiprimes. */
        size_t idx = win_cnt & 7;
        uint64_t old = win_arr[idx];
        win_arr[idx] = cand;
        if (win_cnt >= 8) win_sum -= (seq_t)old;
        win_sum += (seq_t)cand;
        win_cnt++;

        /* If window complete, check average. */
        if (win_cnt >= 8 && (win_sum & 7ULL) == 0) {
            seq_t avg = win_sum / 8;
            if (is_prime_big(avg))
                append_term(avg);
        }
    }

    return seq_cache[n - 1];
}
