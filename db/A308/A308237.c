/*
 * A308237  Numbers m not ending with 0 that contain a digit, other than the
 *          leftmost digit, that can be removed such that the resulting number
 *          d divides m.
 * https://oeis.org/A308237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, changed
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 93, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Validated: full, pass, 95 terms through n = 95 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A308237 by brute‑force search up to 200000 and stores the
   results in a static cache.  The sequence is then returned by index
   (1‑based) from the cache. */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

/* Test whether m satisfies the definition of the sequence. */
static bool satisfies_condition(unsigned long long m)
{
    /* Must not end in 0. */
    if (m % 10 == 0) return false;

    /* Need at least two digits.  m >= 10 always true for our search. */
    char s[32];
    int len = snprintf(s, sizeof s, "%llu", m);
    if (len <= 1) return false;

    for (int pos = 1; pos < len; ++pos) {      /* try removing each non‑left digit */
        unsigned long long d = 0;
        for (int i = 0; i < len; ++i) {
            if (i == pos) continue;
            d = d * 10 + (s[i] - '0');
        }
        if (d != 0 && m % d == 0) return true;
    }
    return false;
}

static seq_t cache[200];
static size_t cached_len = 0;
static bool initialized = false;

/* Build the static cache once. */
static void build_cache(void)
{
    if (initialized) return;
    const unsigned long long UPPER = 200000ULL; /* covers the known maximum 180625 */
    for (unsigned long long m = 11; m <= UPPER; ++m) {
        if (satisfies_condition(m)) {
            if (cached_len < sizeof(cache)/sizeof(cache[0])) {
                cache[cached_len++] = (seq_t)m;
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined behaviour, return 0 for safety */
    build_cache();
    if ((idx_t)cached_len < n) return (seq_t)0;   /* out of bounds: safe fallback */
    return cache[n - 1];
}
