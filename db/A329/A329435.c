/*
 * A329435  Expansion of Sum_{k>=1} (-1 + Product_{j>=2} 1 / (1 - x^(k*j))).
 * https://oeis.org/A329435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (p(d) - p(d-1)), where p(k) is the partition
 *            number of k
 * Range:     u8 n <= 23, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *p_seq = NULL;      /* partition numbers */
static seq_t *a_seq = NULL;      /* a(n) values */
static idx_t cachedN = 0;       /* largest n currently cached */

/* Ensure p_seq and a_seq contain values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n <= cachedN) return;

    /* Allocate new arrays up to n. */
    seq_t *new_p = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *new_a = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!new_p || !new_a) return;   /* allocation failed – unlikely */

    /* Copy old data. */
    if (cachedN > 0) {
        memcpy(new_p, p_seq, (cachedN + 1) * sizeof(seq_t));
        memcpy(new_a, a_seq, (cachedN + 1) * sizeof(seq_t));
    }

    /* Initialize new slots to zero. */
    for (idx_t i = cachedN + 1; i <= n; ++i) {
        new_p[i] = 0;
        new_a[i] = 0;
    }

    /* Base case for partition numbers. */
    if (cachedN == 0) new_p[0] = 1;      /* p(0) = 1 */

    /* Compute partition numbers up to n using the pentagonal‑number recurrence. */
    for (idx_t i = cachedN + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            seq_t term = new_p[i - pent1];
            if (k & 1) sum += term; else sum -= term;

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            term = new_p[i - pent2];
            if (k & 1) sum += term; else sum -= term;
        }
        new_p[i] = sum;
    }

    /* Update a_seq with the convolution of A002865 = p(d)-p(d-1) with 1. */
    /* Contributions from existing divisors for new multiples. */
    for (idx_t d = 1; d <= cachedN; ++d) {
        seq_t val = new_p[d] - new_p[d - 1];
        idx_t start = (cachedN / d + 1) * d;   /* first multiple > cachedN */
        for (idx_t m = start; m <= n; m += d)
            new_a[m] += val;
    }
    /* Contributions from newly added divisors. */
    for (idx_t d = cachedN + 1; d <= n; ++d) {
        seq_t val = new_p[d] - new_p[d - 1];
        for (idx_t m = d; m <= n; m += d)
            new_a[m] += val;
    }

    /* Free old arrays and install new ones. */
    free(p_seq);
    free(a_seq);
    p_seq = new_p;
    a_seq = new_a;
    cachedN = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_cache(n);
    return a_seq[n];
}
