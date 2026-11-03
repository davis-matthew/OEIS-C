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
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   is_prime_u64, mem, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A364320: primes that are the exact average of eight consecutive odd semiprimes.
   Computed by scanning odd integers, testing odd semiprimes, and maintaining a sliding
   window of the last eight. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/smallest_prime_factor.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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

    uint64_t p = smallest_prime_factor(n);
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
        seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
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
