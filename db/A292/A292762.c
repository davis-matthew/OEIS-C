/*
 * A292762  Numbers of the form p^k or 2*p^k, where p is a prime == 3 mod 4
 *          and k is odd.
 * https://oeis.org/A292762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 5069, u32 n <= 9154, u64 n <= 9154,
 *            u128 n <= 9154, big any n
 * Helpers:   append_candidate, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers of the form p^k or 2*p^k, where p≡3 (mod 4) and k is odd. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_spf.h"
#include "../utils/append_candidate.h"

/* Global lowest‑prime‑factor sieve */
static uint64_t *g_spf = NULL;
static idx_t g_spf_limit = 0;

/* Extend the SPF array to cover at least 'need' (inclusive). */
static void ensure_spf(idx_t need) {
    if (need <= g_spf_limit) return;
    idx_t new_limit = g_spf_limit ? g_spf_limit * 2 : 200000;
    while (new_limit < need) new_limit *= 2;
    uint64_t *new_spf = NULL;
    build_spf(new_limit, &new_spf);
    free(g_spf);
    g_spf = new_spf;
    g_spf_limit = new_limit;
}

/* Test whether a 64‑bit integer has the required form. */
static bool is_candidate_uint64(uint64_t val) {
    if (val == 0) return false;

    /* Handle the 2⋅p^k case. */
    if ((val & 1) == 0) {               /* even */
        if ((val & 3) == 0) return false; /* divisible by 4 */
        val >>= 1;                       /* divide by 2 */
    }
    if (val <= 1) return false;          /* p must be prime ≥3 */

    if (g_spf == NULL) {
        build_spf(200000, &g_spf);
        g_spf_limit = 200000;
    }
    if (val > (uint64_t)g_spf_limit) {
        ensure_spf((idx_t)val);
    }

    uint64_t n = val;
    uint64_t p = g_spf[n];
    if (p == 0) return false;          /* should not happen */

    int exp = 0;
    while (n % p == 0) {
        n /= p;
        exp++;
    }
    if (n != 1) return false;          /* more than one prime factor */

    return ((exp & 1) != 0) && ((p & 3) == 3);
}

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined, but safe */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_to_test = 1;

    while (cache_len < (size_t)n) {
        if (is_candidate_uint64(next_to_test)) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)next_to_test);
        }
        next_to_test++;
    }
    return cache[n - 1];
}
