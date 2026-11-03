/*
 * A253576  Primes p such that digits of p do not appear in p^7.
 * https://oeis.org/A253576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) consists of all primes p with the property that none of
 * their decimal digits occurs in the decimal expansion of p^7.
 * The function scans primes one by one, collects those that satisfy
 * the property and returns the n‑th one (1‑based).  The results are
 * memoised so that random access to any indices works in O(1) after the
 * required primes have been found. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

static void digit_freq_uint64(uint64_t x, unsigned int freq[10])
{
    /* Count decimal digits of x. */
    if (x == 0) {
        freq[0] = 1;
        return;
    }
    memset(freq, 0, sizeof(freq[0]) * 10);
    while (x > 0) {
        freq[x % 10]++;
        x /= 10;
    }
}

static void freq_p7_from_prime(uint64_t p, unsigned int freq[10])
{
    /* Compute the decimal digit frequencies of p^7.  The routine
     * represents the number in an array of decimal digits (little‑endian)
     * and multiplies it by p six times (since we start with p^1). */
    int digits[512];
    size_t len = 0;
    uint64_t u = p;

    /* Initialise the array with the decimal digits of p. */
    if (u == 0) {
        memset(freq, 0, sizeof(freq[0]) * 10);
        freq[0] = 1;
        return;
    }
    while (u > 0) {
        digits[len++] = u % 10;
        u /= 10;
    }

    /* Multiply 6 times by p to obtain p^7. */
    for (int m = 0; m < 6; ++m) {
        unsigned long long carry = 0ULL;
        for (size_t j = 0; j < len; ++j) {
            unsigned long long prod = (unsigned long long)digits[j] * p + carry;
            digits[j] = prod % 10;
            carry = prod / 10;
        }
        while (carry > 0) {
            digits[len++] = carry % 10;
            carry /= 10;
        }
    }

    /* Build the frequency array. */
    memset(freq, 0, sizeof(freq[0]) * 10);
    for (size_t j = 0; j < len; ++j)
        freq[digits[j]]++;
}

seq_t a(idx_t n)
{
    /* Static cache of discovered terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 1;   /* start looking after 1 */

    /* Grow cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Get the next prime > last_checked. */
        last_checked = next_prime(last_checked);
        uint64_t p = last_checked;            /* underlying value as uint64_t */
        seq_t p_seq = (seq_t)p;                /* value to store */

        /* Frequency of digits of p. */
        unsigned int freq_p[10];
        digit_freq_uint64(p, freq_p);

        /* Frequency of digits of p^7. */
        unsigned int freq_p7[10];
        freq_p7_from_prime(p, freq_p7);

        /* Check for disjointness of the two digit sets. */
        bool ok = true;
        for (int d = 0; d < 10 && ok; ++d)
            if (freq_p[d] && freq_p7[d])
                ok = false;

        if (ok) {
            /* Ensure capacity. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(1);   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = p_seq;
        }
    }

    return cache[n - 1];
}
