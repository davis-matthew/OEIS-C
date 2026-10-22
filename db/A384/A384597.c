/*
 * A384597  Integers k such that k + 1 has a divisor that is an anagram of k,
 *          which must have the same number of digits as k.
 * https://oeis.org/A384597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   digit_frequencies_uint64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A384597.
   a(n) is the nth integer k (for n ≥ 1) such that k+1 has a divisor
   that is an anagram of k and has the same number of digits as k.  */

/* ---------- standard headers ---------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- utilities used ---------- */
#include "../utils/factor_u64.h"
#include "../utils/digit_frequencies_uint64.h"

/* ---------- helper functions for digit statistics ----------
   They operate on uint64_t values and use only integer operations
   allowed in the build.  */
static int digit_count_uint64(uint64_t n)
{
    int c = 0;
    do { n /= 10; c++; } while (n);
    return c;
}

/* ---------- recursion over all divisors of kp1 ----------
   kp1 = k+1.  The arrays pr and ex contain the prime factors
   and their exponents.  cnt is the number of distinct primes.  */
static void try_divisors(const uint64_t *pr, const uint64_t *ex,
                         size_t cnt, size_t pos, uint64_t curr,
                         int len_k, const unsigned int *freq_k,
                         bool *found)
{
    if (*found) return;

    if (pos == cnt) {
        /* curr is a divisor of kp1 */
        if (digit_count_uint64(curr) == len_k) {
            unsigned int freq_d[10];
            digit_frequencies_uint64(curr, freq_d);
            bool same = true;
            for (int i = 0; i < 10; i++) {
                if (freq_d[i] != freq_k[i]) { same = false; break; }
            }
            if (same) *found = true;
        }
        return;
    }

    uint64_t p = pr[pos];
    uint64_t mul = 1;
    for (uint64_t e = 0; e <= ex[pos]; e++) {
        try_divisors(pr, ex, cnt, pos + 1, curr * mul,
                     len_k, freq_k, found);
        mul *= p;
    }
}

/* ---------- test whether a given k satisfies the condition ---------- */
static bool satisfies(uint64_t k)
{
    uint64_t kp1 = k + 1;

    /* statistics of k */
    unsigned int freq_k[10];
    digit_frequencies_uint64(k, freq_k);
    int len_k = digit_count_uint64(k);

    /* factor kp1 */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(kp1, pr, ex, &cnt);

    bool found = false;
    try_divisors(pr, ex, cnt, 0, 1, len_k, freq_k, &found);
    return found;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 0;

    if (n == 0) return 0;

    /* lazy initialise cache */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        ++last_checked;
        if (satisfies(last_checked)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
