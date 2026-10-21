/*
 * A291042  One powerful arithmetic progression with nontrivial difference and
 *          maximal length.
 * https://oeis.org/A291042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 3^24 * 5^30 * 11^24 * 17^20 * (8*n - 7) (n ≥ 1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Lazy, thread‑free initialization of the constant C = 3^24 * 5^30 * 11^24 * 17^20 */
static seq_t C = (seq_t)0;
static bool initialized = false;

static void init_C(void) {
    if (initialized) return;
    seq_t c = (seq_t)1;
    /* 3^24  */
    for (idx_t i = 0; i < 24; ++i) c *= (seq_t)3;
    /* 5^30  */
    for (idx_t i = 0; i < 30; ++i) c *= (seq_t)5;
    /* 11^24 */
    for (idx_t i = 0; i < 24; ++i) c *= (seq_t)11;
    /* 17^20 */
    for (idx_t i = 0; i < 20; ++i) c *= (seq_t)17;
    C = c;
    initialized = true;
}

seq_t a(idx_t n) {
    init_C();
    /* compute the 8n‑7 term in idx_t and cast to seq_t */
    idx_t t = 8 * n - 7;
    return C * (seq_t)t;
}
