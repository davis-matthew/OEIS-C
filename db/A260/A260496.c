/*
 * A260496  Number of (n+2)X(3+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 00000011 or 00001011.
 * https://oeis.org/A260496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 27, u64 n <= 61, u128 n <= 130,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 197 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation:
   a(n) is the number of (n+2)×5 binary arrays such that
   every 3×3 subblock has a perimeter pattern that is one
   of the 8‑bit strings 00000001, 00000011, 00001011,
   up to rotation (i.e. any cyclic shift of the bits). 
   The function computes a(n) for n≥1 with memoisation. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Return the 8‑bit pattern of the perimeter of a 3×3 block with rows
   r1,r2,r3 (5‑bit values) and left column index c (0–2). */
static inline uint8_t block_perimeter(uint8_t r1, uint8_t r2, uint8_t r3, int c)
{
    uint8_t b0 = (r1 >> c)          & 1; /* top‑left     */
    uint8_t b1 = (r1 >> (c + 1))    & 1; /* top‑middle   */
    uint8_t b2 = (r1 >> (c + 2))    & 1; /* top‑right    */
    uint8_t b3 = (r2 >> (c + 2))    & 1; /* right‑middle */
    uint8_t b4 = (r3 >> (c + 2))    & 1; /* bottom‑right */
    uint8_t b5 = (r3 >> (c + 1))    & 1; /* bottom‑middle*/
    uint8_t b6 = (r3 >> c)          & 1; /* bottom‑left  */
    uint8_t b7 = (r2 >> c)          & 1; /* left‑middle  */
    return (b0 << 7) | (b1 << 6) | (b2 << 5) | (b3 << 4) |
           (b4 << 3) | (b5 << 2) | (b6 << 1) | b7;
}

/* Rotate a byte left by k positions. */
static inline uint8_t rotl8(uint8_t v, int k)
{
    return ((v << k) | (v >> (8 - k))) & 0xFF;
}

#define ROWS 5
#define STATES (1 << (ROWS * 2))   /* 5‑bit rows for two consecutive rows */
#define MAX_STATES 1024            /* 2^10 = 1024 */
#define MAX_R3 32
#define ALLOWED_MASKS 256

/* Transition table: for each state (r1,r2) store all next states (r2,r3). */
static int  nextState[MAX_STATES][MAX_R3];
static uint8_t nextCnt[MAX_STATES];

/* DP arrays for the current and next height. */
static seq_t dpPrev[MAX_STATES];
static seq_t dpCurr[MAX_STATES];

/* Cache of already computed sequence terms (1‑based offset). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t max_cached_n = 0;   /* largest n for which cache[n‑1] is defined */

/* Flag indicating that the transition table has been built. */
static bool trans_ready = false;

/* Table of allowed 8‑bit patterns. */
static uint8_t allowed[ALLOWED_MASKS];

/* Build the set of allowed patterns (three base patterns with all rotations). */
static void init_allowed_patterns(void)
{
    /* Already built? */
    static bool built = false;
    if (built) return;
    for (int i = 0; i < ALLOWED_MASKS; ++i) allowed[i] = 0;
    const uint8_t base[] = {0x01, 0x03, 0x0B};
    for (int b = 0; b < 3; ++b) {
        uint8_t v = base[b];
        for (int k = 0; k < 8; ++k) {
            allowed[rotl8(v, k)] = 1;
        }
    }
    built = true;
}

/* Build the transition table once. */
static void init_transition(void)
{
    if (trans_ready) return;
    init_allowed_patterns();
    for (int s = 0; s < MAX_STATES; ++s) {
        uint8_t r1 = (s >> 5) & 0x1F;  /* high 5 bits */
        uint8_t r2 = s & 0x1F;         /* low 5 bits */
        nextCnt[s] = 0;
        for (uint8_t r3 = 0; r3 < MAX_R3; ++r3) {
            bool ok = true;
            for (int c = 0; c < 3; ++c) {
                uint8_t pat = block_perimeter(r1, r2, r3, c);
                if (!allowed[pat]) { ok = false; break; }
            }
            if (ok) {
                nextState[s][nextCnt[s]++] = (r2 << 5) | r3; /* new state */
            }
        }
    }
    trans_ready = true;
}

seq_t a(idx_t n)
{
    /* Ensure the transition table exists. */
    init_transition();

    /* Allocate cache big enough for n terms (n is 1‑based). */
    if (n > cache_cap) {
        ensure_cache(n, &cache, &cache_cap);
    }

    /* Initialise base DP if this is the first call. */
    if (max_cached_n == 0) {
        for (int s = 0; s < MAX_STATES; ++s) dpPrev[s] = (seq_t)1;
    }

    /* Generate terms until we have reached the desired n. */
    while (max_cached_n < (size_t)n) {
        /* Zero dpCurr. */
        for (int i = 0; i < MAX_STATES; ++i) dpCurr[i] = (seq_t)0;

        /* Transition step: add a new row. */
        for (int s = 0; s < MAX_STATES; ++s) {
            seq_t val = dpPrev[s];
            if (val == (seq_t)0) continue;
            uint8_t cnt = nextCnt[s];
            for (uint8_t k = 0; k < cnt; ++k) {
                int nxt = nextState[s][k];
                dpCurr[nxt] += val;
            }
        }

        /* Move to the new state. */
        for (int i = 0; i < MAX_STATES; ++i) dpPrev[i] = dpCurr[i];

        /* Compute the term corresponding to this height. */
        seq_t total = (seq_t)0;
        for (int i = 0; i < MAX_STATES; ++i) total += dpPrev[i];

        /* Store in cache: cache index = n‑1 = current term number */
        cache[max_cached_n] = total;
        ++max_cached_n;
    }

    return cache[n - 1];
}
