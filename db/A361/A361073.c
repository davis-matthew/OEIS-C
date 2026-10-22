/*
 * A361073  Lexicographically least increasing sequence of triprimes (A014612)
 *          a(n) such that a(n) - a(n-1) and a(n) + a(n-1) are also triprimes.
 * https://oeis.org/A361073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 8, a(n) = Lexicographically least increasing sequence of
 *            triprimes a(n) such that a(n), a(n)-a(n-1) and a(n)+a(n-1) are
 *            all triprimes. Computation is by simple search using a
 *            pre‑computed flag array for triprimes
 * Range:     u8 n <= 4, u16 n <= 822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   generate_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/generate_spf.h"

/* types supplied by the build */
extern seq_t a(idx_t n);

/* ------------------ Tripprime precomputation ------------------ */

#define INITIAL_LIMIT 2000000U   /* initial sieve size (≈10‑100 % larger than needed) */

static uint32_t *spf = NULL;     /* smallest prime factor array */
static uint8_t  *is_trip = NULL; /* flag: 1 if number is a tripprime, 0 otherwise */
static size_t   cur_limit = 0;   /* presently filled array limit */

/* compute tripprime flags up to 'limit' */
static void build_tripprime_flags(size_t limit)
{
    uint32_t *new_spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!new_spf) abort();
    generate_spf(new_spf, limit);

    uint8_t *cnt = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!cnt) abort();
    cnt[0] = cnt[1] = 0;
    for (size_t i = 2; i <= limit; ++i) {
        cnt[i] = cnt[i / new_spf[i]] + 1;
    }

    uint8_t *new_trip = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!new_trip) abort();
    for (size_t i = 0; i <= limit; ++i) {
        new_trip[i] = (cnt[i] == 3) ? 1 : 0;
    }

    /* free old memory and replace */
    free(spf);
    free(cnt);
    free(is_trip);
    spf     = new_spf;
    is_trip = new_trip;
    cur_limit = limit;
}

/* ensure flags cover at least 'needed' */
static void ensure_limit(size_t needed)
{
    if (needed <= cur_limit) return;
    size_t res = cur_limit;
    if (res == 0) res = INITIAL_LIMIT;
    while (res < needed) res <<= 1;  /* double until big enough */
    build_tripprime_flags(res);
}

/* ------------------ Sequence storage ------------------ */

static seq_t *seq_cache = NULL;   /* dynamic array of computed values */
static size_t seq_cap  = 0;        /* allocated capacity */
static idx_t  seq_len  = 0;        /* number of stored terms */

/* grow seq_cache to at least 'need' elements */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    memcpy(new_arr, seq_cache, seq_len * sizeof(seq_t));
    free(seq_cache);
    seq_cache = new_arr;
    seq_cap   = new_cap;
}

/* ------------------ Main entry ------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined below offset, but harmless */

    /* first access: initialise data structures */
    if (!seq_cache) {
        ensure_limit(INITIAL_LIMIT);
        ensure_seq_capacity(1);
        seq_cache[0] = (seq_t)8;   /* a(1) */
        seq_len = 1;
    }

    /* extend cache until we have term n */
    if (seq_len < n) {
        while (seq_len < n) {
            seq_t last = seq_cache[seq_len - 1];
            seq_t cand = last + (seq_t)1;

            /* search for next admissible candidate */
            for (;;) {
                size_t need = (size_t)(cand + last); /* may overflow, but cast to size_t safely here */
                ensure_limit(need);
                size_t idx_cand   = (size_t)cand;
                size_t idx_diff   = (size_t)(cand - last);
                size_t idx_sum    = (size_t)(cand + last);

                if (is_trip[idx_cand] && is_trip[idx_diff] && is_trip[idx_sum]) {
                    break;
                }
                cand = (seq_t)(cand + (seq_t)1);
            }
            ensure_seq_capacity(seq_len + 1);
            seq_cache[seq_len] = cand;
            ++seq_len;
        }
    }

    return seq_cache[n - 1];
}
