/*
 * A249284  Number of length n+3 0..2 arrays with no four consecutive terms
 *          having the sum of any three elements equal to three times the
 *          fourth.
 * https://oeis.org/A249284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 20, u64 n <= 43, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  OEIS A249284 – Number of length n+3 0..2 arrays with no four
    consecutive terms having the sum of any three elements equal to
    three times the fourth.
    The implementation counts admissible sequences by a finite‑state
    automaton. */

static bool init_done = false;
static bool allowed[27][3];
static unsigned char next_state[27][3];

/* Build transition tables only once. */
static void init_allowed(void)
{
    if (init_done) return;
    for (int a = 0; a < 3; ++a) {
        for (int b = 0; b < 3; ++b) {
            for (int c = 0; c < 3; ++c) {
                int s = a * 9 + b * 3 + c;
                for (int d = 0; d < 3; ++d) {
                    bool bad = false;
                    if (a + b + c == 3 * d) bad = true;
                    if (a + b + d == 3 * c) bad = true;
                    if (a + c + d == 3 * b) bad = true;
                    if (b + c + d == 3 * a) bad = true;
                    allowed[s][d] = !bad;
                    next_state[s][d] = (unsigned char)(b * 9 + c * 3 + d);
                }
            }
        }
    }
    init_done = true;
}

/* Cache for already computed terms a(1)…a(n).  cache[0] is unused. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;   /* largest n for which a(n) is stored */

/* Arrays for the DP of the 27 possible triples of last three digits. */
static seq_t dp_prev[27];
static seq_t dp_curr[27];

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, return 0 */

    init_allowed();

    /* Ensure the cache can hold a(n). */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failure */
        /* copy existing data */
        for (size_t i = 0; i <= cache_len; ++i) {
            new_cache[i] = cache ? cache[i] : (seq_t)0;
        }
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* If nothing has been computed yet, start from length 3. */
    if (cache_len == 0) {
        for (int i = 0; i < 27; ++i) dp_prev[i] = (seq_t)1;

        for (size_t len = 4; len <= (size_t)n + 3; ++len) {
            for (int i = 0; i < 27; ++i) dp_curr[i] = (seq_t)0;

            for (int s = 0; s < 27; ++s) {
                seq_t val = dp_prev[s];
                if (val == (seq_t)0) continue;
                for (int d = 0; d < 3; ++d) {
                    if (!allowed[s][d]) continue;
                    int ns = next_state[s][d];
                    dp_curr[ns] += val;
                }
            }

            seq_t total = (seq_t)0;
            for (int i = 0; i < 27; ++i) total += dp_curr[i];

            cache[(size_t)len - 3] = total;

            for (int i = 0; i < 27; ++i) dp_prev[i] = dp_curr[i];
            ++cache_len;
        }
    }
    /* If a larger n is requested, continue the DP from where we left off. */
    else if ((idx_t)n > cache_len) {
        for (size_t len = cache_len + 4; len <= (size_t)n + 3; ++len) {
            for (int i = 0; i < 27; ++i) dp_curr[i] = (seq_t)0;

            for (int s = 0; s < 27; ++s) {
                seq_t val = dp_prev[s];
                if (val == (seq_t)0) continue;
                for (int d = 0; d < 3; ++d) {
                    if (!allowed[s][d]) continue;
                    int ns = next_state[s][d];
                    dp_curr[ns] += val;
                }
            }

            seq_t total = (seq_t)0;
            for (int i = 0; i < 27; ++i) total += dp_curr[i];

            cache[(size_t)len - 3] = total;

            for (int i = 0; i < 27; ++i) dp_prev[i] = dp_curr[i];
            ++cache_len;
        }
    }

    return cache[(size_t)n];
}
