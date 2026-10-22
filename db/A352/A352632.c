/*
 * A352632  Number of ways to write n as (a+b)*(a+2*b) + c^4 + 2*d^4, where a
 *          is a positive integer, and b,c,d are nonnegative integers.
 * https://oeis.org/A352632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 720, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A352632
 *  a(n) = number of solutions in non‑negative integers a>0, b,c,d with
 *  (a+b)*(a+2b) + c^4 + 2*d^4 = n
 *  (equivalently, number of (u,v,c,d) with 0<u<=v<2u, u*v + c^4 + 2*d^4 = n)
 *  This implementation pre‑computes all counts up to the largest n
 *  requested, using a simple triple nested loop.  The algorithm is
 *  O(n^1.5) and easily fast enough for n≤10000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------- helper for arbitrary‑precision seq_t ----------------- */

#ifdef __cplusplus
static inline seq_t pow4_seq(seq_t x) { seq_t y = x * x; return y * y; }
static seq_t *alloc_seq_array(idx_t sz) { return new seq_t[sz](); }
static void free_seq_array(seq_t *p) { delete[] p; }
#else
static inline seq_t pow4_seq(seq_t x) { seq_t y = x * x; return y * y; }
static seq_t *alloc_seq_array(idx_t sz) { return calloc(sz, sizeof(seq_t)); }
static void free_seq_array(seq_t *p) { free(p); }
#endif

/* -------------- global cache ---------------------------------------- */

static seq_t *global_cache = NULL; /* counts for 0 … max_cached */
static idx_t  max_cached    = 0;    /* largest n for which cache is valid */

/* -------------- recompute cache up to N -------------------------------- */

static void recompute_up_to(idx_t N)
{
    /* allocate a fresh count array, zero‑initialized */
    seq_t *new_cache = alloc_seq_array(N + 1);

    /* ranges for c and d : c^4 <= N, 2*d^4 <= N */
    idx_t cmax = 0;
    while (pow4_seq((seq_t)cmax) <= (seq_t)N) ++cmax;
    --cmax;                 /* last valid index */

    idx_t dmax = 0;
    while (2 * pow4_seq((seq_t)dmax) <= (seq_t)N) ++dmax;
    --dmax;                 /* last valid index */

    /* pre‑compute the small power tables */
    idx_t *c4  = (idx_t *)malloc((cmax + 1) * sizeof(idx_t));
    idx_t *d2d = (idx_t *)malloc((dmax + 1) * sizeof(idx_t));

    c4[0] = 0;
    for (idx_t i = 1; i <= cmax; ++i) c4[i] = (idx_t)pow4_seq((seq_t)i);

    d2d[0] = 0;
    for (idx_t i = 1; i <= dmax; ++i) d2d[i] = (idx_t)(2 * pow4_seq((seq_t)i));

    /* main enumeration of (u,v,c,d) */
    for (idx_t u = 1; u <= N; ++u) {
        idx_t maxv = N / u;
        if (maxv < u) continue;
        idx_t limitv = (maxv < (2 * u - 1)) ? maxv : (2 * u - 1);

        for (idx_t v = u; v <= limitv; ++v) {
            idx_t prod = u * v;

            for (idx_t ci = 0; ci <= cmax; ++ci) {
                idx_t c_val = c4[ci];
                if (prod + c_val > N) break;  /* c’s are increasing */

                for (idx_t di = 0; di <= dmax; ++di) {
                    idx_t sum = prod + c_val + d2d[di];
                    if (sum > N) break;       /* d’s are increasing */
                    ++new_cache[sum];
                }
            }
        }
    }

    /* free the old cache and install the new one */
    if (global_cache) free_seq_array(global_cache);
    global_cache = new_cache;
    max_cached   = N;

    /* free temporary tables */
    free(c4);
    free(d2d);
}

/* -------------- public sequence function ------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* does not occur for the OEIS offset */
    if (n > max_cached) recompute_up_to(n);
    return global_cache[n];
}
