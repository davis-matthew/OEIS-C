/*
 * A250147  Number of length n+6 0..1 arrays with every seven consecutive
 *          terms having the maximum of some two terms equal to the minimum of
 *          the remaining five terms.
 * https://oeis.org/A250147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0/1 arrays of length n+6 with no length‑7
 *            subarray containing exactly five 1’s (i.e. no subarray with
 *            exactly two 0’s). The array is counted modulo the width of
 *            seq_t. The implementation uses a 6‑bit sliding‑window automaton
 *            and memoises results
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 29, u64 n <= 65, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define STATES (1 << 6)

static seq_t dp[STATES];
static seq_t nextdp[STATES];
static unsigned char pop6[STATES];
static seq_t *cache = NULL;        /* a(1) stored at cache[1] */
static size_t cache_cap = 0;
static idx_t cur_len = 6;          /* length of strings represented by dp */
static bool initialized = false;

/* initialise the DP tables and cache */
static void init_seq(void) {
    if (initialized) return;

    /* pre‑compute popcount for 6‑bit masks */
    for (int m = 0; m < STATES; ++m)
        pop6[m] = (unsigned char)__builtin_popcount(m);

    /* initial state: all 6‑bit prefixes (length 6) are allowed exactly once */
    for (int m = 0; m < STATES; ++m)
        dp[m] = 1;

    cur_len = 6;             /* strings of length 6 */

    /* allocate cache large enough for at least one term */
    cache_cap = 256;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    memset(cache, 0, cache_cap * sizeof(seq_t));

    initialized = true;
}

/* ensure cache has room for index upto idx (1‑based) */
static void ensure_cache(size_t idx) {
    if (idx < cache_cap) return;
    size_t newcap = cache_cap;
    while (idx >= newcap) newcap *= 2;
    seq_t *newc = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newc) abort();   /* memory error */
    memset(newc + cache_cap, 0, (newcap - cache_cap) * sizeof(seq_t));
    cache = newc;
    cache_cap = newcap;
}

/* extend DP by one position, update cache if needed */
static void advance(void) {
    memset(nextdp, 0, sizeof(nextdp));

    for (int mask = 0; mask < STATES; ++mask) {
        seq_t val = dp[mask];
        if (!val) continue;

        int ones = pop6[mask];

        /* append 0 */
        if (ones != 5) {
            int newmask = (mask << 1) & (STATES - 1);
            nextdp[newmask] += val;
        }
        /* append 1 */
        if (ones + 1 != 5) {
            int newmask = ((mask << 1) & (STATES - 1)) | 1;
            nextdp[newmask] += val;
        }
    }

    memcpy(dp, nextdp, sizeof(dp));
    ++cur_len;    /* now dp represents strings of length cur_len */

    /* compute total for this length */
    seq_t total = 0;
    for (int m = 0; m < STATES; ++m) total += dp[m];

    /* store in cache (a(1) corresponds to length 7) */
    ensure_cache(cur_len - 6);
    cache[cur_len - 6] = total;
}

/* public function */
seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for n<1 */

    init_seq();

    idx_t target_len = n + 6;     /* required string length */
    while (cur_len < target_len) advance();

    return cache[n];
}
