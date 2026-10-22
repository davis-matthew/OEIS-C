/*
 * A347943  Number of ordered pairs (m,k) such that A000009(m) + A000009(k) =
 *          n.
 * https://oeis.org/A347943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ordered pairs (m,k) with
 *            A000009(m)+A000009(k)=n. Computed by direct enumeration of all
 *            m,k up to a fixed bound. The bound is chosen large enough that
 *            A000009(m) > 10000 for all m > 250, thus covering all n ≤ 10000
 * Range:     u8 none, u16 n <= 695, u32 n <= 9749, u64 n <= 9749,
 *            u128 n <= 9749, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int initialized = 0;              /* flag to build the table once */
static const size_t MAX_M = 250;          /* enough for A000009(m) > 10000 */
static seq_t *q = NULL;                  /* A000009 values up to MAX_M */

/* initialise the tables for A000009(0)...A000009(MAX_M) */
static void init_tables(void)
{
    if (initialized) return;
    q = (seq_t *)malloc((MAX_M + 1) * sizeof(seq_t));
    if (!q) return;                /* malloc failure – return zero values */
    for (size_t i = 0; i <= MAX_M; ++i) q[i] = 0;
    q[0] = 1;                       /* partition of 0 is 1 */
    for (size_t k = 1; k <= MAX_M; ++k) {
        for (size_t n = MAX_M; n >= k; --n) {
            q[n] += q[n - k];
        }
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    init_tables();
    seq_t ans = 0;
    /* iterate over all ordered pairs (m,k) with 0 ≤ m,k ≤ MAX_M */
    for (size_t m = 0; m <= MAX_M; ++m) {
        seq_t vm = q[m];
        if (vm > n) continue;                 /* skip m whose value already exceeds n */
        for (size_t k = 0; k <= MAX_M; ++k) {
            seq_t vk = q[k];
            if (vm + vk == n) ans += 1;
        }
    }
    return ans;
}
