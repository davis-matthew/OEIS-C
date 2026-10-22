/*
 * A353504  Number of integer partitions of n whose product is less than the
 *          product of their multiplicities.
 * https://oeis.org/A353504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of integer partitions of n whose product is less than the
 *            product of their multiplicities, computed by enumerating the
 *            multiplicity vectors of parts ≥ 2. Each partition is described
 *            by multiplicities mi (i ≥ 2). Let S = Σ i*mi (total of parts ≥
 *            2) and R = ∏ (i^mi / mi). Remaining 1's count...
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

/* Recursive search: i is current part value being considered,
   sum is Σ i' * m_i' for all i' < i, R is current product ratio.
   count accumulates the number of valid partitions. */
static void dfs(idx_t n, idx_t i, idx_t sum, double R, seq_t *count)
{
    /* pruning: if current R is already not smaller than remaining 1's,
       further additions cannot restore inequality (both R grows and m1 shrinks) */
    const double limit = (double)(n - sum);          /* m1 */
    if (R >= limit - 1e-12)          /* small epsilon to guard against round‑off */
        return;

    /* no more parts ≥ 2 can be added */
    if (i > n - sum) {
        (*count)++;
        return;
    }

    /* try all multiplicities of part i (starting with 0) */
    for (idx_t mi = 0; /* no boundary, will break inside */; ++mi) {
        idx_t newSum = sum + i * mi;
        if (newSum > n - 1) /* ensure at least one 1 remains */
            break;

        double newR = R;
        if (mi > 0)
            newR *= pow((double)i, mi) / mi;  /* R * (i^mi / mi) */

        /* if inequality fails with this choice, larger mi will worsen it */
        if (newR >= (double)(n - newSum) - 1e-12)
            break;

        dfs(n, i + 1, newSum, newR, count);
    }
}

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    /* Base cases handled automatically by the recursion; they return 0. */
    seq_t result = 0;
    dfs(n, 2, 0, 1.0, &result);
    return result;
}
