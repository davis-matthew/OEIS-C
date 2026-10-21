/*
 * A261614  Numbers that are neither prime (A000040) nor practical (A005153).
 * https://oeis.org/A261614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the n‑th positive integer that is neither prime
   nor practical (A261614).  The function searches sequentially,
   skipping primes (is_prime) and practical numbers (is_practical_uint64),
   and memoises the results.  It works for any build of seq_t, including
   arbitrary‑precision integer classes. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* ---------- Practical number test for uint64_t ----------
   Practical numbers satisfy the condition that, for their prime
   factorisation sorted increasingly, each prime p_i (i>1) is not
   larger than the sum of all divisors of the product of the
   preceding prime powers, plus one.  This test runs in O(sqrt n). */
static bool is_practical_uint64(uint64_t n)
{
    if (n == 1) return true;          /* 1 is practical */
    if ((n & 1) != 0) return false;   /* all >1 practical are even */

    uint64_t m = n;
    int exp2 = 0;
    while ((m & 1) == 0) {            /* count powers of 2 */
        m >>= 1;
        ++exp2;
    }

    /* sigma(2^exp2) = 2^(exp2+1) – 1 */
    __uint128_t sigma = ((__uint128_t)1 << (exp2 + 1)) - 1;
    __uint128_t prod   = sigma;       /* running product of sigmas */

    /* factor the remaining odd part */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            /* practical condition */
            if ((__uint128_t)p > prod + 1) return false;
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* sigma(p^exp) */
            uint64_t sigma_pe = 1, cur = 1;
            for (int i = 0; i < exp; ++i) {
                cur *= p;
                sigma_pe += cur;
            }
            prod *= sigma_pe;
        }
    }
    if (m > 1) {          /* remaining prime factor */
        if ((__uint128_t)m > prod + 1) return false;
        prod *= (m + 1);
    }
    return true;
}

/* ---------- Memoisation infrastructure ---------- */
static seq_t *cache   = NULL;       /* cache[0] unused */
static size_t cache_len = 0;        /* 0 means uninitialised */
static uint64_t last_checked = 1;   /* last integer examined */

/* Extend cache so that a(n) is available for all indices <= n. */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;          /* offset 1 – no value needed for 0 */

    /* Initialise cache on first call */
    if (cache_len == 0) {
        cache_len = 1;                            /* keep index 0 */
        cache = (seq_t*)realloc(NULL, sizeof(seq_t) * 2);   /* indices 0,1 */
    }

    if ((size_t)n < cache_len) return; /* already available */

    /* Allocate up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
    cache = tmp;

    /* Generate missing terms */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        do {
            ++last_checked;
        } while (is_prime((idx_t)last_checked) || is_practical_uint64(last_checked));
        cache[i] = (seq_t)last_checked;  /* cast works for built‑in and class types */
    }
    cache_len = new_len;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];   /* cache[0] unused; indices start at 1 */
}
