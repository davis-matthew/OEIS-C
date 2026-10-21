/*
 * A290001  Numbers k such that the iteration k -> (sigma(k)+phi(k))/2
 *          terminates at a fraction later than at the first step.
 * https://oeis.org/A290001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the nth term of OEIS A290001
 *  Sequence: Numbers k such that the iteration
 *  k -> (σ(k)+φ(k))/2 terminates at a fraction
 *  later than at the first step.
 *  The function returns the n‑th term (n≥1) in seq_t.
 *  It uses the provided sigma_of(idx_t) and phi(idx_t) helpers.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/phi.h"

/*  Returns true if k (k>0) eventually leads to a fraction
    after at least one integer step.  */
static bool leads_to_fraction_after_first(idx_t k)
{
    /* First step: (σ(k)+φ(k))/2 */
    seq_t sigma = sigma_of(k);
    seq_t phi_k = phi(k);
    seq_t sum   = sigma + phi_k;

    /* If the first step already gives a fraction (odd sum),
       then k is a square or twice a square and must be excluded.  */
    if (sum % 2 != 0)
        return false;

    idx_t cur = (idx_t)(sum / 2);

    /* Detect cycles: keep all seen integers up to a small limit.  */
    const size_t MAX_STEPS = 200;
    idx_t seen[MAX_STEPS];
    size_t seen_cnt = 0;

    while (1) {
        /* Cycle detection: if cur repeats, no fraction occurs.  */
        for (size_t i = 0; i < seen_cnt; ++i)
            if (seen[i] == cur)
                return false;

        if (seen_cnt < MAX_STEPS)
            seen[seen_cnt++] = cur;

        /* Next step */
        sigma = sigma_of(cur);
        phi_k = phi(cur);
        sum   = sigma + phi_k;

        if (sum % 2 != 0)        /* fraction found later */
            return true;

        cur = (idx_t)(sum / 2);
    }
}

/*  Returns the nth term of the sequence.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n = 0, but keep simple */

    idx_t count = 0;
    idx_t k     = 1;

    while (true) {
        if (leads_to_fraction_after_first(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
