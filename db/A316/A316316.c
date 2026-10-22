/*
 * A316316  Coordination sequence for tetravalent node in chamfered version of
 *          square grid.
 * https://oeis.org/A316316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + 2*a(n-3) - a(n-4) + a(n-5) - a(n-6) for
 *            n > 6, with a(0)=1, a(1)=4, a(2)=8, a(3)=8, a(4)=12, a(5)=20,
 *            a(6)=20
 * Range:     u8 n <= 76, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;      /* Cached sequence values. */
static idx_t  seq_len   = 0;         /* Number of values currently stored. */
static idx_t  seq_cap   = 0;         /* Capacity of the cache array. */

/* Ensure the cache is initialized with the first seven terms. */
static void ensure_initialized(void) {
    if (seq_cache != NULL) return;          /* Already initialized. */
    seq_cap = 64;                            /* Initial capacity. */
    seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (seq_cache == NULL) return;           /* Allocation failed – nothing to do. */
    seq_cache[0] = 1;
    seq_cache[1] = 4;
    seq_cache[2] = 8;
    seq_cache[3] = 8;
    seq_cache[4] = 12;
    seq_cache[5] = 20;
    seq_cache[6] = 20;
    seq_len   = 7;
}

/* Grow the cache to at least size `needed`.  Uses doubling strategy. */
static void grow_cache(idx_t needed) {
    if (seq_cap >= needed) return;
    if (seq_cap < 16) seq_cap = 16;
    while (seq_cap <= needed) seq_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
}

/* Public function computing a(n). */
seq_t a(idx_t n) {
    ensure_initialized();

    /* Grow the cache if necessary. */
    if (n >= seq_len) {
        grow_cache(n + 1);
        for (idx_t i = seq_len; i <= n; ++i) {
            /* Compute a(i) using the recurrence. */
            seq_t an1 = seq_cache[i-1];
            seq_t an2 = seq_cache[i-2];
            seq_t an3 = seq_cache[i-3];
            seq_t an4 = seq_cache[i-4];
            seq_t an5 = seq_cache[i-5];
            seq_t an6 = seq_cache[i-6];
            seq_cache[i] = an1 - an2 + 2*an3 - an4 + an5 - an6;
        }
        seq_len = n + 1;
    }

    return seq_cache[n];
}
