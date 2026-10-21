/*
 * A249702  Number of length n+3 0..3 arrays with every four consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A249702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 25, u64 n <= 55, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A249702 by dynamic programming over the last three values.
   The transition table is built once (O(4^4)).  The function works in both
   C and C++ builds; for C++ the cache is a std::vector, for C it is a
   malloc‑based array (realloc).  All arithmetic uses the type seq_t.
   No external libraries beyond stdlib.h/stdint.h/stdbool.h are needed. */

/* ------   common includes   ------ */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* -----  state transition data  ----- */
static bool tables_inited = false;
static idx_t current_len = 3;                     /* processed length, >=3 */
static seq_t curr[64];
static seq_t next[64];
static int  next_state[64][4];                    /* -1 if transition forbidden */

/* -----  cache of computed values  ----- */
#ifdef __cplusplus
static std::vector<seq_t> cache;
static void ensure_cache_capacity(size_t needed)
{
    if (cache.size() < needed)
        cache.resize(needed);                    /* value 0 is used for new entries */
}
#else
static seq_t *cache = NULL;
static size_t cache_len = 0;                      /* actual number of stored entries (>=1) */
static size_t cache_cap = 0;                      /* allocated capacity */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}
#endif

/* ----------  build transition table  ---------- */
static void init_tables(void)
{
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            for (int c = 0; c < 4; ++c) {
                int s = a * 16 + b * 4 + c;     /* state index */
                for (int d = 0; d < 4; ++d) {
                    int p[4] = {a, b, c, d};
                    bool ok = false;
                    for (int i = 0; i < 4 && !ok; ++i)
                        for (int j = i + 1; j < 4 && !ok; ++j) {
                            int mx = p[i] > p[j] ? p[i] : p[j];
                            int mn = 4;
                            for (int k = 0; k < 4; ++k)
                                if (k != i && k != j && p[k] < mn)
                                    mn = p[k];
                            if (mx == mn) ok = true;
                        }
                    if (ok)
                        next_state[s][d] = b * 16 + c * 4 + d;
                    else
                        next_state[s][d] = -1;
                }
            }
    tables_inited = true;
}

/* ----------  main sequence routine  ---------- */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;           /* undefined for n<1 */

    /* First‑time initialisation */
    if (!tables_inited) {
        init_tables();
        for (int i = 0; i < 64; ++i) curr[i] = (seq_t)1;   /* all 3‑tuples */
    }

    /* extend DP until we have processed length n+3 */
    idx_t target_len = n + 3;
    while (current_len < target_len) {
        /* zero next counts */
        for (int i = 0; i < 64; ++i) next[i] = (seq_t)0;

        /* transitions */
        for (int s = 0; s < 64; ++s) {
            seq_t cnt = curr[s];
            if (cnt == 0) continue;
            for (int d = 0; d < 4; ++d) {
                int ns = next_state[s][d];
                if (ns >= 0) next[ns] += cnt;
            }
        }

        ++current_len;                       /* now length is current_len */

        if (current_len >= 4) {              /* compute a(cur_n) */
            idx_t cur_n = current_len - 3;
            seq_t sum = (seq_t)0;
            for (int i = 0; i < 64; ++i) sum += next[i];

            ensure_cache_capacity(cur_n);
#ifdef __cplusplus
            cache[cur_n-1] = sum;
#else
            cache[cur_n-1] = sum;
            if (cur_n > cache_len) cache_len = cur_n;
#endif
        }

        /* prepare for next iteration */
        for (int i = 0; i < 64; ++i) curr[i] = next[i];
    }

    /* cache[n-1] now holds a(n) */
    return cache[n-1];
}
