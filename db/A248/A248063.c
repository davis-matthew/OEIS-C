/*
 * A248063  Number of length n+5 0..3 arrays with some disjoint triples in
 *          each consecutive six terms having the same sum.
 * https://oeis.org/A248063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 23, u64 n <= 57, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A248063: Number of length n+5 0..3 arrays with some disjoint triples
   in each consecutive six terms having the same sum.
   Computed by the empirical recurrence (see OEIS). */
#include <stdlib.h>
#include <stdbool.h>

/* Empirical recurrence
   a(n) = 2*a(n-1) - a(n-4) + a(n-5) + a(n-6)
          - 2*a(n-7) - 2*a(n-8) + 2*a(n-10)
          - a(n-12) + a(n-13) + a(n-15) - a(n-16)
   Initial values a(1)..a(16) are given in the comments. */

static bool valid_window[4096];
static void init_valid(void)
{
    static const int triples[10][3] = {
        {0,1,2}, {0,1,3}, {0,1,4}, {0,1,5},
        {0,2,3}, {0,2,4}, {0,2,5},
        {0,3,4}, {0,3,5},
        {0,4,5}
    };
    int all[6] = {0,1,2,3,4,5};

    for (int code = 0; code < 4096; ++code) {
        int tmp = code;
        int d[6];
        for (int i = 5; i >= 0; --i) {
            d[i] = tmp & 3;
            tmp >>= 2;
        }
        bool ok = false;
        for (int p = 0; p < 10 && !ok; ++p) {
            int sum1 = d[triples[p][0]] + d[triples[p][1]] + d[triples[p][2]];
            bool seen[6] = {0};
            for (int k = 0; k < 3; ++k) seen[triples[p][k]] = 1;
            int sum2 = 0;
            for (int i = 0; i < 6; ++i)
                if (!seen[i]) sum2 += d[i];
            if (sum1 == sum2) ok = true;
        }
        valid_window[code] = ok;
    }
}

/* 4^5 = 1024 states (last 5 digits of the current prefix) */
#define STATECOUNT 1024

static seq_t dp_cur[STATECOUNT];
static seq_t dp_next[STATECOUNT];
static idx_t len_cur = 5;          /* current processed length (prefix length)  */
static bool dp_initialized = false;

/* Cache of a(n) values for computed n */
static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;
static idx_t a_cache_last = 0;     /* largest n whose a(n) is stored */

/* Ensure that a_cache can hold values up to n (inclusive) */
static void ensure_cache_cap(idx_t n)
{
    if ((size_t)(n + 1) <= a_cache_size) return;
    size_t new_size = a_cache_size ? a_cache_size * 2 : 32;
    while ((size_t)(n + 1) > new_size) new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;
    a_cache_size = new_size;
}

/* initialise the dynamic programming tables */
static void init_dp(void)
{
    if (dp_initialized) return;
    if (!valid_window[0]) init_valid();   /* ensures valid_window is filled */
    for (int s = 0; s < STATECOUNT; ++s) dp_cur[s] = 1;  /* all 5‑digit prefixes allowed initially */
    /* compute a(1) to a(16) using recurrence */
    static const seq_t init_vals[17] = {
        0ULL,
        1804ULL, 3364ULL, 6364ULL, 12124ULL, 23164ULL,
        44284ULL, 84604ULL, 161824ULL, 309724ULL, 592924ULL,
        1135114ULL, 2173054ULL, 4159984ULL, 7963744ULL,
        15245944ULL, 29187364ULL
    };
    ensure_cache_cap(16);
    for (idx_t i = 1; i <= 16; ++i) a_cache[i] = init_vals[i];
    a_cache_last = 16;
    dp_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0ULL;
    init_dp();

    /* return cached value if already computed */
    if (n <= a_cache_last) return a_cache[n];

    /* compute further values up to n */
    while (a_cache_last < n) {
        /* advance one length step: len_cur -> len_cur+1 */
        for (int i = 0; i < STATECOUNT; ++i) dp_next[i] = 0;
        for (int state = 0; state < STATECOUNT; ++state) {
            if (!dp_cur[state]) continue;
            for (int d = 0; d < 4; ++d) {
                int code = (state << 2) + d;   /* window of 6 digits */
                if (!valid_window[code]) continue;
                int new_state = ((state << 2) & 0x3FF) | d;
                dp_next[new_state] += dp_cur[state];
            }
        }
        /* copy back */
        for (int i = 0; i < STATECOUNT; ++i) dp_cur[i] = dp_next[i];
        ++len_cur;           /* len_cur now equals previous + 1 */

        /* the current length is len_cur, so corresponding n = len_cur-5 */
        idx_t cur_n = len_cur - 5;
        /* sum over all states to get a(cur_n) */
        seq_t total = 0;
        for (int i = 0; i < STATECOUNT; ++i) total += dp_cur[i];
        ensure_cache_cap(cur_n);
        a_cache[cur_n] = total;
        a_cache_last = cur_n;
    }

    return a_cache[n];
}
