/*
 * A389591  Sum of the divisors r of n such that r is not greater than twice
 *          the adjacent previous divisor of n.
 * https://oeis.org/A389591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A389591:
   a(n) = σ(n) – Σ (smallest divisor of each maximal 2‑dense block)   */
/* Input:   n ≥ 1 (offset 1)
   Output:  the required sum as seq_t
   Complexity: O(√n + dlogd) where d is the number of divisors of n
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety for accidental 0 */
    idx_t N = n;

    /* an array large enough for all divisors of n (≤ 2*sqrt(N)) */
    uint64_t divs[256];
    size_t cnt = 0;

    /* collect divisors */
    for (idx_t i = 1; i <= N / i; ++i)
    {
        if (N % i == 0)
        {
            divs[cnt++] = i;
            idx_t j = N / i;
            if (j != i)
                divs[cnt++] = j;
        }
    }

    /* sort divisors */
    qsort(divs, cnt, sizeof(uint64_t), cmp_uint64);

    seq_t sumAll = 0;
    seq_t sumSmallest = 0;
    uint64_t prev = 0;

    /* scan sorted divisors */
    for (size_t idx = 0; idx < cnt; ++idx)
    {
        uint64_t d = divs[idx];
        seq_t d_seq = (seq_t)d;

        sumAll += d_seq;

        /* start of a new 2‑dense block */
        if (idx == 0 || d > 2 * prev)
            sumSmallest += d_seq;

        prev = d;
    }

    return sumAll - sumSmallest;
}
