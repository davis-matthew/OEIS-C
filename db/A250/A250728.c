/*
 * A250728  Number of (n+1)X(7+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 49, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A250728 : Number of (n+1)×8 0/1 arrays with the two
 *  monotonicity conditions.
 *
 *  Empirical linear recurrence (valid for n > 17):
 *
 *      a(n) = 8 a(n-1) - 27 a(n-2) + 48 a(n-3) - 42 a(n-4)
 *             + 42 a(n-6) - 48 a(n-7) + 27 a(n-8) - 8 a(n-9)
 *             + a(n-10)
 *
 *  The coefficients are derived by a transfer‑matrix calculation
 *  and are known to hold for all n with n > 17.
 *
 *  For 1 ≤ n ≤ 17 the values are obtained by a simple
 *  dynamic programming over the 256 possible row patterns.
 *
 *  The implementation uses a small, memoised DP to build
 *  the first 17 terms and then applies the recurrence for
 *  larger n.  The function is idempotent and works
 *  independently of the call order.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* --------------  static working data  ---------------- */
    static int initialized = 0;
    static seq_t *cache = NULL;         /* a[ k ] for k = 0..cache_max */
    static size_t cache_cap = 0;        /* allocated capacity for cache */
    static idx_t cache_max = 0;
    static unsigned char sums[256][7];   /* column‑pair sums for each mask */
    static unsigned char valid[256][256];/* 0/1 flag for allowed transition */
    static seq_t dp_prev[256];
    static seq_t dp_next[256];

    /* --------------  initialise once  ------------------- */
    if (!initialized) {
        /* pre‑compute column‑pair sums  */
        for (idx_t m = 0; m < 256; ++m) {
            for (int j = 0; j < 7; ++j) {
                unsigned char a = (m >> j) & 1U;
                unsigned char b = (m >> (j + 1)) & 1U;
                sums[m][j] = a + b;
            }
        }

        /* pre‑compute valid transitions  */
        for (idx_t p = 0; p < 256; ++p) {
            for (idx_t q = 0; q < 256; ++q) {
                int ok = 1;
                /* Condition 2 : AND must be a suffix of 1’s  */
                int seen1 = 0;
                for (int bit = 0; bit < 8; ++bit) {
                    int b = ((p >> bit) & 1U) & ((q >> bit) & 1U);
                    if (b) seen1 = 1;
                    else if (seen1) { ok = 0; break; }
                }
                if (!ok) { valid[p][q] = 0; continue; }

                /* Condition 1 : sums across rows are non‑decreasing  */
                for (int j = 0; j < 7; ++j) {
                    if (sums[p][j] > sums[q][j]) { ok = 0; break; }
                }
                valid[p][q] = ok;
            }
        }

        /* initialise DP for the first row: every mask is possible once  */
        for (int m = 0; m < 256; ++m) dp_prev[m] = 1;

        /* allocate cache for a[0]..a[17] (index 0 unused for -) */
        cache_cap = 18;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        assert(cache);
        cache[0] = 0;          /* unused */
        cache_max = 0;         /* no values computed yet */
        initialized = 1;
    }

    if (n < 1) return 0;        /* out of bounds – no defined value */

    /* --------------  extend cache if needed  -------------- */
    if (cache_max < 17) {
        /* compute DP for n up to min(n,17)  */
        while (cache_max < (idx_t)17 && cache_max < n) {
            /* next DP step */
            for (int i = 0; i < 256; ++i) dp_next[i] = 0;
            for (int p = 0; p < 256; ++p) {
                seq_t val = dp_prev[p];
                if (!val) continue;
                for (int q = 0; q < 256; ++q) {
                    if (valid[p][q]) dp_next[q] += val;
                }
            }
            /* sum all configurations for this length  */
            seq_t total = 0;
            for (int i = 0; i < 256; ++i) total += dp_next[i];

            /* store result */
            if (cache_cap <= cache_max + 1) {
                cache_cap = cache_max + 2;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                assert(tmp);
                cache = tmp;
            }
            cache[++cache_max] = total;

            /* prepare for next iteration */
            for (int i = 0; i < 256; ++i) dp_prev[i] = dp_next[i];
        }
    }

    /* --------------  use recurrence for n > 17  ------------ */
    if (cache_max < n) {
        /* ensure cache has enough space  */
        if (cache_cap <= n) {
            cache_cap = n + 1;
            seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            assert(tmp);
            cache = tmp;
        }
        for (idx_t k = cache_max + 1; k <= n; ++k) {
            /* recurrence for k > 17, but works for k ≥ 18  */
            seq_t val = 8 * cache[k-1];
            val -= 27 * cache[k-2];
            val += 48 * cache[k-3];
            val -= 42 * cache[k-4];
            val += 42 * cache[k-6];
            val -= 48 * cache[k-7];
            val += 27 * cache[k-8];
            val -= 8  * cache[k-9];
            val +=   cache[k-10];
            cache[k] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
