/*
 * A329110  Number of integer sequences 1 <= b_1 < b_2 < ... < b_t <= n such
 *          that b_i divides b_(i+1) for all 0 < i < t.
 * https://oeis.org/A329110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} g(k), where g(1) = 1 g(k) = 1 + sum_{d|k,
 *            d<k} g(d) This gives a(1)=1, a(2)=3, a(3)=5, a(4)=9, …
 *            (cumulative sum of the number of chains ending at each k)
 * Range:     u8 n <= 31, u16 n <= 791, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"

static seq_t *garr = NULL;           /* g(k) values, 1‑based */
static seq_t *carr = NULL;           /* cumulative a(n) values, 1‑based */
static idx_t  cap  = 0;              /* allocated capacity >= current max index + 1 */
static idx_t  last = 0;              /* last computed index */

/* Ensure storage large enough for indices up to n (1‑based).  */
static void need_capacity(idx_t n)
{
    if (cap > n) return;
    idx_t new_cap = (cap == 0 ? 16 : cap);
    while (new_cap <= n) new_cap <<= 1;
    seq_t *g_new = (seq_t*)realloc(garr, new_cap * sizeof(seq_t));
    seq_t *c_new = (seq_t*)realloc(carr, new_cap * sizeof(seq_t));
    if (!g_new || !c_new) exit(1);   /* allocation failure */
    /* Zero the newly allocated part. */
    if (new_cap > cap) {
        memset(g_new + cap, 0, (new_cap - cap) * sizeof(seq_t));
        memset(c_new + cap, 0, (new_cap - cap) * sizeof(seq_t));
    }
    garr = g_new;
    carr = c_new;
    cap  = new_cap;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* 0 for undefined n<1 */

    need_capacity(n);
    /* Compute missing values up to n. */
    for (idx_t k = last + 1; k <= n; ++k) {
        if (k == 1) {
            garr[1] = (seq_t)1;
            carr[1] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            idx_t sqrtk = _floor_isqrt_u64(k);
            for (idx_t d = 1; d <= sqrtk; ++d) {
                if (k % d == 0) {
                    if (d < k)                 /* proper divisor */
                        sum += garr[d];
                    idx_t other = k / d;
                    if (other != d && other < k)  /* complementary divisor */
                        sum += garr[other];
                }
            }
            garr[k] = (seq_t)1 + sum;
            carr[k] = carr[k - 1] + garr[k];
        }
    }
    last = n;
    return carr[n];
}
