/*
 * A276425  Sum of the even singletons in all partitions of n (n>=0). A
 *          singleton in a partition is a part that occurs exactly once.
 * https://oeis.org/A276425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k even ≥ 2} k * (p(n−k) − p(n−2k)) where p(m) is the
 *            partition function. The algorithm precomputes p[0..N] with the
 *            standard O(N²) coin‑change DP. For each call it then loops over
 *            even parts k to accumulate the contribution. The routine is
 *            memoised, so repeated calls share the partition...
 * Range:     u8 n <= 12, u16 n <= 32, u32 n <= 99, u64 n <= 346,
 *            u128 n <= 1290, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *partitions = NULL;   /* p[0..cached_n] */
static idx_t   cached_n   = 0;

/* Recompute partition numbers up to N.  Complexity O(N²). */
static void ensure_partitions(idx_t N)
{
    if (N <= cached_n) return;

    /* Allocate new table; zero it, set p[0] = 1. */
    seq_t *newp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!newp) exit(1);
    for (idx_t i = 0; i <= N; ++i) newp[i] = 0;
    newp[0] = 1;

    /* Standard coin‑change DP: each part size j contributes to all sums i >= j. */
    for (idx_t j = 1; j <= N; ++j)
        for (idx_t i = j; i <= N; ++i)
            newp[i] += newp[i - j];

    /* Replace old table. */
    free(partitions);
    partitions = newp;
    cached_n = N;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    if (n < 2) return 0;          /* no even singleton possible */

    ensure_partitions(n);         /* make sure p[0..n] are ready */

    for (idx_t k = 2; k <= n; k += 2)
    {
        seq_t pk1 = partitions[n - k];          /* p(n−k)    */
        seq_t pk2 = (n >= 2 * k) ? partitions[n - 2 * k] : 0; /* p(n−2k)  */
        seq_t diff = pk1 - pk2;                  /* non‑negative */
        result += (seq_t)k * diff;
    }
    return result;
}
