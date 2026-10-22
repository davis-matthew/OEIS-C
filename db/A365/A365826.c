/*
 * A365826  Number of strict integer partitions of n that are not of length 2
 *          and do not contain n/2.
 * https://oeis.org/A365826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Q(n) - ⌊(n-1)/2⌋ - [n even && n>0]*(Q(n/2)-1) where Q(n)
 *            is the number of strict (distinct part) partitions of n. The
 *            function computes Q(n) by dynamic programming and then applies
 *            the formula
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *Q_cache = NULL;   /* Q_cache[i] = Q(i) */
static idx_t Q_cache_len = 0;   /* number of elements stored (max n + 1) */

/* Recompute Q(0..N) using the standard distinct-part DP (O(N^2)). */
static void recompute_Q_upto(idx_t N)
{
    seq_t *new_cache = (seq_t *)realloc(Q_cache, (size_t)(N + 1) * sizeof(seq_t));
    if (!new_cache) exit(1);
    Q_cache = new_cache;
    Q_cache_len = N + 1;

    /* initialise to 0 */
    for (idx_t i = 0; i <= N; ++i) Q_cache[i] = 0;
    Q_cache[0] = 1;                 /* one partition of 0 – the empty partition */

    /* distinct‑partitions DP */
    for (idx_t part = 1; part <= N; ++part) {
        for (idx_t sum = N; sum >= part; --sum) {
            Q_cache[sum] += Q_cache[sum - part];
        }
    }
}

/* Ensure that Q_cache[0..N] is available. */
static void ensure_Q(idx_t N)
{
    if ((idx_t)(N + 1) <= Q_cache_len) return;
    recompute_Q_upto(N);
}

/* OEIS sequence A365826 */
seq_t a(idx_t n)
{
    ensure_Q(n);

    seq_t res = Q_cache[n];

    /* subtract length‑2 partitions: floor((n‑1)/2)  (0 for n<1) */
    seq_t len2 = 0;
    if (n >= 1) len2 = (seq_t)((n - 1) / 2);
    res -= len2;

    /* subtract partitions that contain part n/2 (only for even n>0) */
    if (n >= 2 && (n & 1ULL) == 0) {
        seq_t part_cnt = Q_cache[n / 2];
        /* exclude the single‑part partition [n/2] itself */
        if (part_cnt > 0) res -= (part_cnt - 1);
    }

    return res;
}
