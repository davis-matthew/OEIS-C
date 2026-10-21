/*
 * A247926  Number of length n+4 0..7 arrays with some disjoint pairs in every
 *          consecutive five terms having the same sum.
 * https://oeis.org/A247926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 20, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247926: number of length n+4 arrays over {0,…,7}
   with, in every consecutive five terms, a pair of disjoint
   pairs having the same sum.
   Implemented by a transfer‑matrix DP over the last four digits.
   DP state: a 4‑digit suffix (a,b,c,d) → number of ways.
   An allowed next digit e is pre‑checked: the 5‑tuple a,b,c,d,e
   must contain two disjoint pairs with equal sum.  All arithmetic
   is performed using seq_t; for the arbitrary‑precision build
   seq_t is a C++ class, for the standard builds it is an unsigned
   integer.  The routine keeps a cache so that multiple calls are
   fast and independent of call order. */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Number of states: 8⁴ = 4096. */
#define STATES 4096

/* Encode 4 digits: s = a*512 + b*64 + c*8 + d.  lower3[s] holds
   the lower three digits (b,c,d). */
static uint16_t lower3[STATES];
/* For each state s, a bitmask indicating which next digit e (0‑7)
   keeps the disjoint‑pair condition satisfied. */
static uint8_t valid_mask[STATES];

/* DP arrays (current and next).  Allocated lazily in init_tables. */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* Cache of results: cache[n] = a(n).  cache[0] unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  cached_up_to = 0;

/* Pre‑compute lower3, valid_mask and allocate DP arrays. */
static void init_tables(void) {
    static bool initialized = false;
    if (initialized) return;
    initialized = true;
    for (idx_t s = 0; s < STATES; ++s) {
        lower3[s] = (uint16_t)(s & 0x1FF);          /* s % 512 */
        int a = s / 512;
        int b = (s / 64) % 8;
        int c = (s / 8 ) % 8;
        int d = s % 8;
        uint8_t mask = 0;
        for (int e = 0; e < 8; ++e) {
            int digits[5] = {a, b, c, d, e};
            bool ok = false;
            for (int i = 0; i < 4 && !ok; ++i)
                for (int j = i + 1; j < 5 && !ok; ++j) {
                    int sum = digits[i] + digits[j];
                    for (int i2 = 0; i2 < 4 && !ok; ++i2)
                        for (int j2 = i2 + 1; j2 < 5 && !ok; ++j2)
                            if (sum == digits[i2] + digits[j2] &&
                                i != i2 && i != j2 && j != i2 && j != j2)
                                ok = true;
                }
            if (ok) mask |= (1U << e);
        }
        valid_mask[s] = mask;
    }
    /* Allocate DP arrays once. */
    dp_cur  = (seq_t*)calloc(STATES, sizeof(seq_t));
    dp_next = (seq_t*)calloc(STATES, sizeof(seq_t));
    for (idx_t s = 0; s < STATES; ++s)
        dp_cur[s] = (seq_t)1;           /* length‑4 prefixes */
}

/* Grow the cache to hold at least indices 0…n. */
static void ensure_cache_cap(idx_t n) {
    if ((size_t)(n + 1) <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    if (new_cache != cache) cache = new_cache;
    memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* a(0) is undefined; return 0 */
    init_tables();

    if (cached_up_to < n) {
        ensure_cache_cap(n);
        for (idx_t step = cached_up_to + 1; step <= n; ++step) {
            /* Zero dp_next. */
            memset(dp_next, 0, STATES * sizeof(seq_t));

            /* Transition from each state. */
            for (idx_t s = 0; s < STATES; ++s) {
                seq_t val = dp_cur[s];
                if (val == 0) continue;
                uint8_t mask = valid_mask[s];
                for (int e = 0; e < 8; ++e) {
                    if (!(mask & (1U << e))) continue;
                    idx_t dst = ((idx_t)lower3[s] << 3) | (idx_t)e;
                    dp_next[dst] = dp_next[dst] + val;
                }
            }

            /* Swap dp_cur and dp_next. */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;

            /* Sum all states to get a(step). */
            seq_t total = (seq_t)0;
            for (idx_t s = 0; s < STATES; ++s)
                total = total + dp_cur[s];
            cache[step] = total;
        }
        cached_up_to = n;
    }
    return cache[n];
}
