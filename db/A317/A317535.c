/*
 * A317535  Expansion of 1/(1 + 1/(1 - x) - Product_{k>=1} 1/(1 - x^k)).
 * https://oeis.org/A317535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} p(k)·a(n-k) – Σ_{k=1}^{n} a(n-k) where p(k)
 *            is the partition function (p(0)=1) computed by the pentagonal
 *            number theorem. This follows from the inverse relation a(n) =
 *            Σ_{k=1}^{n} (p(k)-1)·a(n-k). All calculations are done with the
 *            type seq_t provided by the build
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 59, u128 n <= 116,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Cast the index to a machine word for array bounds */
    idx_t N = n;
    size_t size = (size_t)N + 1;

    /* Allocate space for the sequence and the partition numbers */
    seq_t *seqA = alloc_seq_array(size);
    seq_t *part = alloc_seq_array(size);
    if (!seqA || !part) {
        /* Allocation failed; return 0 as a graceful degradation. */
        if (seqA) free_seq_array(seqA);
        if (part) free_seq_array(part);
        return (seq_t)0;
    }

    /* Base cases */
    seqA[0] = (seq_t)1;
    part[0] = (seq_t)1;

    /* Compute all terms up to N */
    for (idx_t i = 1; i <= N; ++i) {
        /* Compute partition number part[i] via pentagonal recurrence */
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (idx_t m = 1;; ++m) {
            idx_t g1 = m * (3 * m - 1) / 2;
            if (g1 > i) break;
            seq_t term = part[i - g1];
            if (m & 1) pos += term; else neg += term;

            idx_t g2 = m * (3 * m + 1) / 2;
            if (g2 > i) continue;
            if (m & 1) pos += part[i - g2]; else neg += part[i - g2];
        }
        part[i] = pos - neg;   /* pos ≥ neg holds for partition numbers */

        /* Compute a[i] = Σ p(k)·a(i-k) – Σ a(i-k) */
        seq_t sum_a   = (seq_t)0;
        seq_t sum_pa  = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t aik = seqA[i - k];
            sum_a  += aik;
            sum_pa += part[k] * aik;
        }
        seqA[i] = sum_pa - sum_a;
    }

    seq_t result = seqA[N];

    /* Free allocated memory */
    free_seq_array(seqA);
    free_seq_array(part);

    return result;
}
