/*
 * A319010  a(0) = 0, a(1) = 1; for n >= 1, a(2*n) = a(2*n-1), a(2*n+1) = 2*(n
 *          - a(n)).
 * https://oeis.org/A319010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 510, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) of OEIS A319010
 * Recurrence:
 *   a(0) = 0, a(1) = 1
 *   for n >= 1:
 *     a(2*n)   = a(2*n-1)
 *     a(2*n+1) = 2 * (n - a(n))
 * This function computes a(n) for all n >= 0.
 * It uses a static cache of sufficient size to handle all required
 * indices (up to 200000, adequate for n <= 10000 as requested).
 */
#include <stddef.h>  /* for size_t */

/* External integer types provided by the build */
extern seq_t a(idx_t n);

static seq_t cache[200000];
static idx_t max_computed = 1;   /* indices 0 and 1 are known */
static int initialized = 0;      /* flag indicating cache is ready */

seq_t a(idx_t n)
{
    /* one‑time cache initialization */
    if (!initialized) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        initialized = 1;
    }

    /* compute any missing values up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if ((i & 1) == 0) {                     /* even index */
                cache[i] = cache[i - 1];
            } else {                                 /* odd index */
                idx_t m = i >> 1;                    /* m = (i-1)/2 */
                seq_t sm = (seq_t)m;                 /* cast to seq_t */
                cache[i] = 2 * (sm - cache[m]);      /* 2*(m - a(m)) */
            }
        }
        max_computed = n;
    }

    return cache[n];
}
