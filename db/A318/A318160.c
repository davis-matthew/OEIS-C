/*
 * A318160  Number of compositions of n into exactly n nonnegative parts with
 *          largest part ceiling(n/2).
 * https://oeis.org/A318160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of weak compositions of n into n non‑negative
 *            parts * with maximal part = ceil(n/2). The function uses
 *            inclusion–exclusion * to count compositions with all parts ≤ B:
 *            * * f(B) = Σ_{k=0}^{⌊n/(B+1)⌋} (-1)^k · C(n,k) · C(2n - k·(B+1)
 *            - 1, n-1) * * a(n) = f(ceil(n/2)) – f(ceil(n/2)-1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Author: ChatGPT
 *  Date: 2026‑10‑20
 */

#include <stdint.h>
#include "../utils/binom.h"   /* binom(idx_t n, idx_t k) -> seq_t */

/* Count compositions of n into n parts where each part ≤ B. */
static seq_t compose_bound(idx_t n, idx_t B)
{
    /* trivial cases */
    if (n == 0)            /* empty composition */
        return 1;
    if (B == 0)
    {
        /* With non‑negative parts bounded by 0, the only possible
           composition is all zeros, hence the sum is n.  If n>0
           there is no such composition. */
        return 0;
    }

    /* the number of terms in the inclusion–exclusion sum */
    idx_t limit = n / (B + 1);

    /* k = 0 term */
    seq_t result = binom(2 * n - 1, n - 1);

    if (limit >= 1)
    {
        /* k = 1 term (subtraction) */
        seq_t t1 = binom(n, 1) *
                   binom(n - (B + 1) + n - 1, n - 1);
        result -= t1;

        if (limit >= 2)
        {
            /* k = 2 term (addition) */
            seq_t t2 = binom(n, 2) *
                       binom(n - 2 * (B + 1) + n - 1, n - 1);
            result += t2;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* ceil(n/2) */
    idx_t U = (n + 1) / 2;

    seq_t fU  = compose_bound(n, U);
    seq_t fU1 = compose_bound(n, U - 1);

    return fU - fU1;
}
