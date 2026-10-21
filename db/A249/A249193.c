/*
 * A249193  Number of length n+6 0..4 arrays with no seven consecutive terms
 *          having four times the sum of any three elements equal to three
 *          times the sum of the remaining four.
 * https://oeis.org/A249193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 23, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249193 – Number of length n+6 arrays over {0..4} with no 7‑term window
   such that 4·(sum of any 3 entries) = 3·(sum of the remaining 4).
   Computation uses dynamic programming over the last 6 digits.
   State space: 5^6 = 15625 possible 6‑digit suffixes.
   For each suffix we precompute which next digit keeps the window valid.
   Then we propagate counts for increasing length.  a(1) corresponds to
   arrays of length 7.  The routine is memoised and works for arbitrary
   order of calls.  All arithmetic is done on type seq_t; casting for
   memory allocation is performed so that the code compiles in C++ with
   a big‑integer seq_t. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STATE 15625            /* 5^6 */
#define POW5_5 3125            /* 5^5 */

/* For each 6‑digit suffix store which next digit is allowed (true=ok). */
static bool allowed[STATE][5] = {0};
static bool init_allowed = false;

/* Pre‑compute the transition table. */
static void precompute_allowed(void)
{
    if (init_allowed) return;
    init_allowed = true;

    for (int idx = 0; idx < STATE; ++idx) {
        int s[6];
        int tmp = idx;
        for (int pos = 5; pos >= 0; --pos) {
            s[pos] = tmp % 5;
            tmp /= 5;
        }

        for (int d = 0; d < 5; ++d) {
            int w[7];
            for (int i = 0; i < 6; ++i) w[i] = s[i];
            w[6] = d;

            int sum = 0;
            for (int i = 0; i < 7; ++i) sum += w[i];

            bool forbidden = false;
            if (sum % 7 == 0) {
                int target = (3 * sum) / 7;
                for (int i = 0; i < 7 && !forbidden; ++i)
                    for (int j = i + 1; j < 7 && !forbidden; ++j)
                        for (int k = j + 1; k < 7; ++k)
                            if (w[i] + w[j] + w[k] == target) {
                                forbidden = true;
                                break;
                            }
            }
            allowed[idx][d] = !forbidden;
        }
    }
}

/* DP tables and memoisation cache. */
static seq_t *dp      = NULL;
static seq_t *newdp   = NULL;
static size_t dp_len  = 0;        /* current sequence length processed */

static seq_t *cache          = NULL;   /* cache[n] = a(n) for n ≥ 1 */
static size_t cache_cap      = 0;
static size_t cached_max     = 0;       /* highest n already cached */

/* Ensure cache can hold element at index idx (0‑based). */
static void ensure_cache_cap(size_t idx)
{
    if (idx < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= idx) new_cap *= 2;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    /* initialise new slots to zero */
    memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Expand DP until a(n) for the required n is ready. */
static void expand_to(idx_t n)
{
    if (n == 0) return;
    if ((size_t)n <= cached_max) return;

    precompute_allowed();

    if (!dp) {
        dp    = (seq_t*)calloc(STATE, sizeof(seq_t));
        newdp = (seq_t*)calloc(STATE, sizeof(seq_t));
        if (!dp || !newdp) exit(1);
    }

    while (cached_max < (size_t)n) {
        /* Build next length. */
        if (dp_len < 6) {
            dp_len++;                /* lengths 1…6 initialise dp */
            if (dp_len == 6) {
                for (int i = 0; i < STATE; ++i) dp[i] = 1;
            }
            continue;
        }

        /* Transition from length L = dp_len to L+1. */
        memset(newdp, 0, STATE * sizeof(seq_t));

        for (int idx = 0; idx < STATE; ++idx) {
            if (dp[idx] == 0) continue;
            for (int d = 0; d < 5; ++d) {
                if (!allowed[idx][d]) continue;
                int new_idx = (idx % POW5_5) * 5 + d;
                newdp[new_idx] += dp[idx];
            }
        }

        /* swap buffers */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;

        dp_len++;   /* now length = dp_len */

        /* For lengths ≥7, compute a(n). */
        if (dp_len >= 7) {
            size_t curr_n = dp_len - 6;
            seq_t sum = 0;
            for (int i = 0; i < STATE; ++i) sum += dp[i];

            ensure_cache_cap(curr_n);
            cache[curr_n] = sum;
            if (curr_n > cached_max) cached_max = curr_n;
        }
    }
}

/* Public function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* outside offset – returning 0 */
    expand_to(n);
    if ((size_t)n >= cache_cap) return 0;   /* safety guard */
    return cache[n];
}
