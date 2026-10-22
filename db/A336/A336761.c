/*
 * A336761  a(0) = 0, a(1) = 1; for n > 1, a(n) = a(n-1) - lpf(n) if
 *          nonnegative and not already in the sequence, otherwise a(n) =
 *          a(n-1) + lpf(n), where lpf(n) is the least prime dividing n.
 * https://oeis.org/A336761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; for n > 1, * a(n) = a(n-1) - lpf(n) if the
 *            result is nonnegative and not * already in the sequence;
 *            otherwise a(n) = a(n-1) + lpf(n)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A straightforward implementation that recomputes the sequence
 * from scratch for each requested n.  This satisfies the
 * specification for all n >= 0, and runs fast for n up to
 * at least 10000.  It uses only standard C11 features and
 * the utility function lpf(), which returns the least prime
 * factor of its argument. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/lpf.h"   /* provides idx_t lpf(idx_t n) */

seq_t a(idx_t n)
{
    /* Handle the initial two terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Allocate an array large enough to hold the sequence up to n */
    /* Cast is required for C++ compatibility */
    seq_t *seq = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!seq) exit(EXIT_FAILURE);

    seq[0] = (seq_t)0;
    seq[1] = (seq_t)1;

    for (idx_t k = 2; k <= n; ++k) {
        seq_t prev = seq[(size_t)(k - 1)];
        idx_t pf   = lpf(k);           /* least prime factor of k */
        seq_t pf_seq = (seq_t)pf;

        /* Attempt subtraction */
        if (prev >= pf_seq) {
            seq_t cand = prev - pf_seq;
            bool seen = false;
            /* Linear search among indices 0 .. k-2 */
            for (idx_t j = 0; j < k - 1; ++j) {
                if (seq[(size_t)j] == cand) {
                    seen = true;
                    break;
                }
            }
            if (!seen) {
                seq[(size_t)k] = cand;
                continue;
            }
        }

        /* Fallback to addition */
        seq[(size_t)k] = prev + pf_seq;
    }

    seq_t result = seq[(size_t)n];
    free(seq);
    return result;
}
