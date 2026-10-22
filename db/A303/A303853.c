/*
 * A303853  Number of pairs (lambda,mu) of partitions lambda of n and mu of
 *          three with mu <= lambda (by diagram containment).
 * https://oeis.org/A303853
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*p(n) - 2*floor(n/2) - 4 (where p(n) is the partition
 *            number of n)
 * Range:     u8 n <= 12, u16 n <= 37, u32 n <= 117, u64 n <= 398,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ------------- partition number cache ------------------------------------ */
static seq_t *p_cache = NULL;   /* array of p(k) for 0 <= k <= p_max   */
static idx_t  p_max  = 0;       /* current maximum k computed          */

/* enlarge the cache and compute partition numbers up to `limit`
   using the generalized pentagonal recurrence that only uses
   non‑negative intermediate numbers. */
static void compute_partitions(idx_t limit)
{
    size_t new_cap = (size_t)(limit + 1);
    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();               /* allocation failure */

    /* initialize all entries to zero */
    for (idx_t i = 0; i <= limit; ++i) new_arr[i] = (seq_t)0;
    new_arr[0] = (seq_t)1;   /* p(0) = 1 */

    /* generalized pentagonal recurrence without signed intermediates.
       For each k, split terms into positive and negative groups
       to avoid underflow. */
    for (idx_t k = 1; k <= limit; ++k) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (idx_t j = 1; ; ++j) {
            /* g1 = j*(3*j - 1)/2 */
            idx_t g1 = (j * (3 * j - 1)) / 2;
            if (g1 > k) break;

            seq_t term = new_arr[k - g1];
            if (j & 1) pos += term; else neg += term;

            /* g2 = j*(3*j + 1)/2 */
            idx_t g2 = (j * (3 * j + 1)) / 2;
            if (g2 > k) continue;

            term = new_arr[k - g2];
            if (j & 1) pos += term; else neg += term;
        }
        new_arr[k] = pos - neg;   /* pos >= neg for every k */
    }

    /* replace old cache */
    free(p_cache);
    p_cache = new_arr;
    p_max   = limit;
}

static void ensure_partitions(idx_t n)
{
    if (p_cache && n <= p_max) return;
    compute_partitions(n);
}

/* ------------- main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;           /* value for n < 3 is irrelevant */

    /* compute partition number p(n) */
    ensure_partitions(n);
    seq_t pn = p_cache[n];

    /* a(n) = 3*p(n) - 2*floor(n/2) - 4 */
    seq_t half = (seq_t)(n >> 1);          /* floor(n/2) */
    seq_t result = pn * ((seq_t)3);
    result -= half * ((seq_t)2);
    result -= ((seq_t)4);

    return result;
}
