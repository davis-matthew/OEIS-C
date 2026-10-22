/*
 * A316861  a(n) = Sum_{p in P} y(1)*y(2), where P is the set of partitions of
 *          n, and y(k) is the number of parts with multiplicity at least k in
 *          p.
 * https://oeis.org/A316861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 98, u64 n <= 343,
 *            u128 n <= 1000, big any n
 * Helpers:   r_coef
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A316861
    a(n) = Sum_{p ⊢ n} y(1)*y(2)
    where y(k) is the number of parts with multiplicity at least k in partition p
    Formula (OEIS): a(n) = ∑_{k=0}^{n} r(k)·p(n−k)
    r(k) = 0            (k < 2)
           floor((k−3)/2)+1  + [k even] − [k≡0 mod 3]   (k ≥ 2)
    p(m) is the partition function.   */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/r_coef.h"

static seq_t *partitions = NULL;   /* p[0..N]   */
static idx_t  part_size   = 0;     /* current number of stored entries */

/* Ensure that partitions[0..N] are available, computing any missing values. */
static void
ensure_partitions(idx_t N)
{
    if (part_size > N)
        return;   /* already have enough */

    /* Expand array to N+1 entries */
    size_t new_size = (size_t)N + 1;
    seq_t *tmp = (seq_t *)realloc(partitions, new_size * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    partitions = tmp;

    /* Initialise new slots to zero */
    for (size_t i = part_size; i < new_size; ++i)
        partitions[i] = (seq_t)0;

    /* Base partition p(0)=1 */
    if (part_size == 0) {
        partitions[0] = (seq_t)1;
        part_size = 1;
    }

    /* Compute missing partition values using the pentagonal recurrence */
    for (idx_t n = part_size; n <= N; ++n) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > n && g2 > n) break;

            int sign = (k & 1) ? 1 : -1;   /* + for odd k, – for even k */

            if (g1 <= n) {
                if (sign > 0) pos += partitions[n - g1];
                else          neg += partitions[n - g1];
            }
            if (g2 <= n) {
                if (sign > 0) pos += partitions[n - g2];
                else          neg += partitions[n - g2];
            }
        }
        partitions[n] = pos - neg;   /* pos ≥ neg always */
    }

    part_size = new_size;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    ensure_partitions(n);

    seq_t total = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t r = r_coef(k);
        if (r == 0) continue;
        /* multiply partition value by small integer r */
        seq_t term = partitions[n - k] * (seq_t)r;
        total += term;
    }
    return total;
}
