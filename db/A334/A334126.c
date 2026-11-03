/*
 * A334126  a(n) is the smallest positive integer not already in the sequence
 *          such that Sum_{i=k..n} a(i) is not a perfect power for 0 < k < n;
 *          start with a(1)=1.
 * https://oeis.org/A334126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-03)
 * Helpers:   bit_length, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A334126 – a(n) is the smallest positive integer not already in
 * the sequence such that for each 0 < k < n the suffix sum 
 *      Σ_{i=k..n} a(i)
 * is not a perfect power.  a(1) = 1.
 * 
 * Implementation uses a brute‑force search with memoisation.
 * The first value is hard‑coded and subsequent values are generated
 * by testing candidate integers against all existing suffix sums.
 * Perfect powers are detected by checking integer roots up to the
 * bit‑length of the value.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bit_length.h"
#include "../utils/mem.h"

/* power_by_exponent_with_limit: base^exp, but if result would exceed limit
   returns limit+1 instead (so caller can detect overflow). */
static inline seq_t power_with_limit(seq_t base, idx_t exp, seq_t limit)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        if (base == 0) return 0;
        if (result > limit / base) return limit + 1;
        result *= base;
    }
    return result;
}

/* integer kth root (floor of n^(1/k)) */
static inline seq_t kth_root(seq_t n, idx_t k)
{
    if (k == 1) return n;
    seq_t low = 1;
    seq_t high = n;
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        seq_t val = power_with_limit(mid, k, n);
        if (val == n) return mid;
        if (val < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* Helper prototypes ----------------------------------------------------- */
static inline seq_t power_with_limit(seq_t base, idx_t exp, seq_t limit);
static inline seq_t kth_root(seq_t n, idx_t k);
static inline bool is_perfect_power(seq_t x);

/* Memoisation storage ---------------------------------------------------- */
static seq_t *terms     = NULL;   /* a(1)..a(count) */
static size_t terms_cap = 0;
static size_t count     = 0;      /* number of terms generated */

static seq_t *pref     = NULL;   /* prefix sums: pref[0]=0, pref[1]=a1, ... */
static size_t pref_cap = 0;

static bool *used_map = NULL;   /* used_map[v]==true if v has appeared */
static size_t used_cap = 0;

/* Ensure allocator capacity --------------------------------------------- */
static void ensure_terms_cap(size_t need)
{
    if (terms_cap < need) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 8;
        while (new_cap < need) new_cap <<= 1;
        terms = mem_resize(seq_t, terms, new_cap);
        terms_cap = new_cap;
    }
}

static void ensure_pref_cap(size_t need)
{
    if (pref_cap < need) {
        size_t new_cap = pref_cap ? pref_cap * 2 : 8;
        while (new_cap < need) new_cap <<= 1;
        pref = mem_resize(seq_t, pref, new_cap);
        pref_cap = new_cap;
    }
}

static void ensure_used_cap(size_t need)
{
    if (used_cap <= need) {
        size_t new_cap = used_cap ? used_cap * 2 : 16;
        while (new_cap <= need) new_cap <<= 1;
        bool *tmp = (bool*)realloc(used_map, new_cap * sizeof(bool));
        /* initialise new entries to false */
        for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
        used_map = tmp;
        used_cap = new_cap;
    }
}

/* Generate terms up to the given index (1‑based). ------------------------ */
static void compute_to(idx_t target)
{
    /* initialisation on first call */
    if (terms == NULL) {
        ensure_terms_cap(16);
        ensure_pref_cap(17);
        ensure_used_cap(32);
        terms[0] = 1;           /* a(1) = 1 */
        pref[0] = 0;
        pref[1] = 1;
        used_map[1] = true;
        count = 1;
    }

    while (count < (size_t)target) {
        seq_t best = 0;
        seq_t current_sum = pref[count];   /* Σ_{i=1..count} a(i) */

        /* search for smallest unused integer that keeps all suffix sums non‑perfect */
        seq_t cand = 1;
        for (;;) {
            /* skip if already used */
            if ((size_t)cand < used_cap) {
                if (used_map[(size_t)cand]) { ++cand; continue; }
            } else {
                ensure_used_cap((size_t)cand);
                if (used_map[(size_t)cand]) { ++cand; continue; }
            }

            seq_t new_sum = current_sum + cand;
            bool ok = true;
            for (size_t j = 0; j < count && ok; ++j) {
                seq_t diff = new_sum - pref[j]; /* suffix sum from j+1 to count+1 */
                if (is_perfect_power(diff)) ok = false;
            }
            if (ok) { best = cand; break; }
            ++cand;
        }

        /* append chosen value */
        ensure_terms_cap(count + 2);
        terms[count] = best;

        /* update prefix sums */
        ensure_pref_cap(count + 3);
        pref[count + 1] = pref[count] + best;

        /* mark as used */
        ensure_used_cap((size_t)best);
        used_map[(size_t)best] = true;

        ++count;
    }
}

/* --------------------------------------------------------------------- */
/* Public function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety for potential 0‑based calls */
    if ((size_t)n > count) compute_to(n);
    return terms[n - 1];
}

/* --------------------------------------------------------------------- */
/* Helper functions ------------------------------------------------------- */

/* true if x is a perfect power (exponent ≥ 2); 1 is treated as a perfect power. */
static inline bool is_perfect_power(seq_t x)
{
    if (x <= 1) return true;          /* 0 and 1 are perfect powers */

    idx_t maxExp = bit_length(x) - 1;  /* 2^maxExp ≤ x < 2^(maxExp+1) */
    for (idx_t e = 2; e <= maxExp; ++e) {
        seq_t r = kth_root(x, e);
        if (r <= 1) break;           /* further exponents will give root 1 */
        if (power_with_limit(r, e, x) == x) return true;
    }
    return false;
}
