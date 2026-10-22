/*
 * A376424  Nonnegative numbers m such that the run lengths in binary
 *          expansion of m, say (r_1, ..., r_k), satisfy r_1 + ... + r_i <>
 *          r_j + ... + r_k for any i in the interval 1..k-1 and j in the
 *          interval 2..k.
 * https://oeis.org/A376424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 6561, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376424: brute‑force search for numbers m whose binary
   run‑length prefixes are distinct from suffixes. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: check whether a given number m satisfies the
   run‑length prefix/suffix condition. */
static inline bool valid_number(idx_t m)
{
    /* Special case: zero has one run of length 1. */
    if (m == 0) return true;

    /* Find length of binary representation (no leading zeros). */
    int bitlen = 64 - __builtin_clzll((unsigned long long)m);

    /* Store run lengths.  Maximum number of runs is bitlen. */
    unsigned char runs[65];
    int runs_len = 0;

    unsigned int cur = (m >> (bitlen - 1)) & 1U;
    int rl = 1;
    for (int pos = bitlen - 2; pos >= 0; --pos) {
        unsigned int b = (m >> pos) & 1U;
        if (b == cur) {
            ++rl;
        } else {
            runs[runs_len++] = rl;
            cur = b;
            rl = 1;
        }
    }
    runs[runs_len++] = rl;  /* last run */

    if (runs_len == 1) return true;  /* only one run → vacuously satisfied */

    /* Prefix and suffix sums of run lengths. */
    seq_t pref[65];
    seq_t suff[65];

    pref[0] = (seq_t)runs[0];
    for (int i = 1; i < runs_len; ++i)
        pref[i] = pref[i - 1] + (seq_t)runs[i];

    suff[runs_len - 1] = (seq_t)runs[runs_len - 1];
    for (int i = runs_len - 2; i >= 0; --i)
        suff[i] = suff[i + 1] + (seq_t)runs[i];

    /* Check for equality between any prefix (except full) and any suffix
       (except starting at the first run). */
    for (int i = 0; i < runs_len - 1; ++i) {
        for (int j = 1; j < runs_len; ++j) {
            if (pref[i] == suff[j])
                return false;
        }
    }
    return true;
}

/* Static cache of generated terms.  It grows on demand. */
static seq_t *cache = NULL;
static size_t cap   = 0;
static size_t len   = 0;
static idx_t next_m = 0;

/* Generate terms up to the nth term (1‑based). */
static void generate_terms_to(idx_t n)
{
    if (n <= len) return;

    while (len < n) {
        while (!valid_number(next_m))
            ++next_m;

        /* Ensure enough space. */
        if (len == cap) {
            size_t new_cap = cap == 0 ? 16 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }

        cache[len++] = (seq_t)next_m;
        ++next_m;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    generate_terms_to(n);
    return cache[n - 1];
}
