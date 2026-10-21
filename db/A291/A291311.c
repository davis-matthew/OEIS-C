/*
 * A291311  Expansion of (1-x^2)/((1-x-x^2)*(1-x-x^4)).
 * https://oeis.org/A291311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-3) + a(n-4) - a(n-5) - a(n-6);
   with initial values a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=9, a(5)=16 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache    = NULL;   /* array holding computed terms */
static idx_t  cap      = 0;      /* allocated capacity of cache     */
static idx_t  count    = 0;      /* number of terms actually computed */

/* Ensure cache has enough space for indices up to n */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cap) return;          /* already large enough */

    idx_t new_cap = n + 1;
    seq_t *new_buf;

#ifdef __cplusplus
    new_buf = new seq_t[new_cap];
    if (cache) {
        for (idx_t i = 0; i < count; ++i) new_buf[i] = cache[i];
        delete[] cache;
    }
#else
    new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif

    if (!new_buf) return;              /* allocation failed – leave old cache */
    cache = new_buf;
    cap   = new_cap;
}

/* Compute terms up to and including n, storing them in cache */
static void fill_up_to(idx_t n)
{
    if (n < 6) return;                /* base values handled elsewhere */

    if (count > n) return;            /* already computed */

    for (idx_t i = count; i <= n; ++i) {
        if (i < 6) {                  /* initial values */
            switch (i) {
                case 0: cache[i] = (seq_t)1; break;
                case 1: cache[i] = (seq_t)2; break;
                case 2: cache[i] = (seq_t)3; break;
                case 3: cache[i] = (seq_t)5; break;
                case 4: cache[i] = (seq_t)9; break;
                case 5: cache[i] = (seq_t)16; break;
            }
        } else {                      /* recurrence for i ≥ 6 */
            seq_t val = cache[i-1];
            val = val + val;           /* 2*a(i-1) */
            val = val - cache[i-3];    /* -a(i-3) */
            val = val + cache[i-4];    /* +a(i-4) */
            val = val - cache[i-5];    /* -a(i-5) */
            val = val - cache[i-6];    /* -a(i-6) */
            cache[i] = val;
        }
    }
    if (count <= n) count = n + 1;
}

seq_t a(idx_t n)
{
    /* Return base values directly */
    if (n < 6) {
        static const seq_t base[6] = {
            (seq_t)1, (seq_t)2, (seq_t)3,
            (seq_t)5, (seq_t)9, (seq_t)16
        };
        return base[n];
    }

    ensure_cache(n);
    if (count <= n) fill_up_to(n);
    return cache[n];
}
