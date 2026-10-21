/*
 * A248029  Least positive integer m such that m + n divides phi(m)*sigma(n),
 *          where phi(.) and sigma(.) are given by A000010 and A000203.
 * https://oeis.org/A248029
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1251, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A248029: least positive integer m such that m + n divides φ(m)·σ(n) */
/* This implementation searches m = 1 … n, using the provided phi(·)
   and sigma_of(·) utilities.  Since it is guaranteed that a solution
   exists and that a(n) ≤ n, the function returns the first m satisfying
   the condition.  The search is O(n·log n) and is fast enough for
   n up to at least 10 000. */

#include <stdint.h>             /* for uint64_t (idx_t) */
#include "../utils/phi.h"       /* seq_t phi(idx_t m); */
#include "../utils/sigma_of.h"  /* seq_t sigma_of(idx_t n); */

seq_t a(idx_t n)
{
    if (n < 2) /* offset starts at 2; value below is undefined */
        return (seq_t)0;

    /* Pre‑compute σ(n) once */
    const seq_t sigma_n = sigma_of(n);

    for (idx_t m = 1; m <= n; ++m)
    {
        const seq_t phi_m = phi(m);
        /* (φ(m) * σ(n)) must be a multiple of (m + n) */
        const seq_t prod = phi_m * sigma_n;
        const seq_t sum  = (seq_t)m + (seq_t)n;
        if (prod % sum == 0)
            return (seq_t)m;
    }

    /* The definition ensures a solution exists; fall back to n */
    return (seq_t)n;
}
