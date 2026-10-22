/*
 * A350137  Nonsquarefree numbers whose prime signature, except possibly the
 *          first and last parts, is all even.
 * https://oeis.org/A350137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A350137: nth nonsquarefree integer whose internal prime‑exponent pattern
   (except possibly the first and last) consists solely of even numbers.
   The implementation searches upward, using a smallest‑prime‑factor sieve
   to factor candidates efficiently, and caches results so that calls to
   a(idx_t n) are O(n) overall. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* SPF table and its capacity */
static uint32_t *global_spf = NULL;
static size_t global_spf_cap = 0;

/* build or rebuild SPF table up to "limit" inclusive */
static void build_spf(size_t limit)
{
    if (global_spf) {
        free(global_spf);
    }
    global_spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!global_spf) return; /* allocation failure – unlikely */

    if (limit >= 2) {
        for (size_t i = 2; i <= limit; ++i) {
            if (global_spf[i] == 0) {
                global_spf[i] = (uint32_t)i;
                if (i > limit / i) continue; /* guard i*i overflow */
                for (size_t j = i * i; j <= limit; j += i) {
                    if (global_spf[j] == 0) global_spf[j] = (uint32_t)i;
                }
            }
        }
    }
    global_spf_cap = limit;
}

/* Determine whether x is a member of A350137. */
static bool qualifies(idx_t x)
{
    /* 4 is the only nonsquarefree <4 */
    if (x < 4) return x == 4;

    size_t tmp = (size_t)x;
    if (tmp > global_spf_cap) return false;

    int exp[32];
    int ecnt = 0;

    while (tmp > 1) {
        uint32_t p = global_spf[tmp];
        int cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        exp[ecnt++] = cnt;
        if (ecnt >= 32) break;   /* safety guard */
    }

    /* must be nonsquarefree: some exponent >= 2 */
    bool nonsq = false;
    for (int i = 0; i < ecnt; ++i) {
        if (exp[i] >= 2) { nonsq = true; break; }
    }
    if (!nonsq) return false;

    /* if only one or two primes, there are no middle exponents */
    if (ecnt <= 2) return true;

    for (int i = 1; i <= ecnt - 2; ++i) {
        if (exp[i] & 1) return false; /* odd middle exponent */
    }
    return true;
}

/* sequence cache */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static idx_t last_checked = 3;   /* start searching from 4 */

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but harmless */

    /* Generate terms lazily until we have n of them */
    while (seq_len < (size_t)n) {
        idx_t cand = last_checked + 1;

        /* Ensure SPF table covers the new candidate */
        if ((size_t)cand > global_spf_cap) {
            size_t new_limit = (size_t)cand * 2;
            if (new_limit < (size_t)cand) new_limit = (size_t)cand + 1000;
            build_spf(new_limit);
        }

        if (qualifies(cand)) {
            /* Grow the cache if necessary */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                seq_t *tmp = seq_realloc(seq_cache, seq_cap, new_cap);
                if (!tmp) return 0; /* allocation failure – unlikely */
                seq_cache = tmp;
                seq_cap = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)cand;
        }
        last_checked = cand;
    }
    return seq_cache[(size_t)n - 1];
}
