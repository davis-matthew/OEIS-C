/*
 * A369516  Numbers k in A126706 such that neither k-1 nor k+1 is in A126706.
 * https://oeis.org/A369516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A369516:
 *  a(n) = the n-th integer k such that k is in A126706
 *  (k is not square‑free and not a prime power) and neither k−1 nor k+1
 *  is in A126706.
 *  The algorithm builds a sieve of smallest prime factors, tests each
 *  candidate using that sieve, and caches the found terms.  It runs in
 *  O(N log log N) time for the first N terms and answers any query in O(1)
 *  afterwards.  It works for all builds (uint8_t … __int128, or an
 *  arbitrary‑precision class); the only special C++ requirement is the
 *  cast of the result of malloc / realloc to a pointer to `seq_t`.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- sieve of smallest prime factors (spf) ---------- */
static uint32_t *spf = NULL;   /* spf[i] = smallest prime divisor of i */
static size_t spf_size = 0;    /* current sieve limit */

/* rebuild spf array up to 'limit' */
static void recompute_spf(size_t limit)
{
    free(spf);
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) exit(1);
    for (size_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;
    for (size_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (size_t j = i * i; j <= limit; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_size = limit;
}

/* enlarge sieve if necessary */
static void ensure_spf(size_t n)
{
    if (n <= spf_size) return;
    size_t new_lim = n;
    if (new_lim < 1024) new_lim = 1024;
    recompute_spf(new_lim);
}

/* ---------- test membership in A126706 ---------- */
static bool is_in_a126706(uint64_t x)
{
    if (x < 2) return false;
    ensure_spf((size_t)x);                /* make sure spf covers x   */
    if (spf[(size_t)x] == (uint32_t)x)    /* prime -> not in A126706 */
        return false;

    int distinct = 0;
    bool has_square = false;
    uint64_t n = x;
    while (n > 1) {
        uint32_t p = spf[(size_t)n];
        int exp = 0;
        while (n % p == 0) { n /= p; ++exp; }
        if (exp >= 2) has_square = true;
        ++distinct;
        if (distinct >= 2 && has_square) break;    /* early stop */
    }
    return distinct >= 2 && has_square;
}

/* ---------- sequence state and generation ---------- */
static seq_t *term_cache = NULL;   /* stored a(n) values */
static size_t term_cap   = 0;      /* capacity of term_cache */
static size_t term_count = 0;      /* number of stored terms   */
static uint64_t next_candidate = 12;   /* next integer to test */

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* initialise sieve once */
    if (spf_size == 0) recompute_spf(2000000);

    /* initialise cache storage */
    if (!term_cache) {
        term_cap = 4;
        term_cache = (seq_t *)malloc(term_cap * sizeof(seq_t));
        if (!term_cache) exit(1);
    }

    /* generate terms until we have at least n of them */
    while (term_count < (size_t)n) {
        uint64_t k = next_candidate;
        ensure_spf((size_t)(k + 2));                 /* also cover k+1 */

        bool good = false;
        if (is_in_a126706(k)) {
            bool left  = (k > 1)  && is_in_a126706(k-1);
            bool right =           is_in_a126706(k+1);
            good = !left && !right;
        }

        if (good) {
            if (term_count == term_cap) {
                term_cap *= 2;
                term_cache = (seq_t *)realloc(term_cache, term_cap * sizeof(seq_t));
                if (!term_cache) exit(1);
            }
            term_cache[term_count++] = (seq_t)k;
        }
        ++next_candidate;
    }

    return term_cache[n-1];
}
