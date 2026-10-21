/*
 * A297199  a(n) = number of partitions of n into consecutive positive cubes.
 * https://oeis.org/A297199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into consecutive positive cubes. *
 *            a(n) = |{ (k,m) : 1 ≤ k ≤ m, ∑_{i=k}^{m} i^3 = n }| * For fixed
 *            k, sum_{i=k}^{m} i^3 = (m(m+1)/2)^2 – ((k-1)k/2)^2. * We count
 *            all pairs (k,m) with k^3 ≤ n and m ≥ k that satisfy this
 *            equality
 * Range:     u8 n <= 3747, u16 n <= 9581, u32 n <= 9581, u64 n <= 9581,
 *            u128 n <= 9581, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t A[256];
static size_t A_len = 0;
static seq_t A_max = 0;

static void ensure_A_upto(seq_t limit)
{
    if (A_len == 0)
    {
        /* fill first value for m=1 */
        size_t m = 1;
        seq_t t = (seq_t)m * (m + 1) / 2;
        A[0] = t * t;
        A_len = 1;
        A_max = A[0];
    }
    while (A_len < 256 && A_max < limit)
    {
        size_t m = A_len + 1;
        seq_t t = (seq_t)m * (m + 1) / 2;
        seq_t val = t * t;
        A[A_len++] = val;
        A_max = val;
    }
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t sn = (seq_t)n;
    /* iterate over possible starting cube k */
    for (idx_t k = 1;; ++k)
    {
        seq_t k3 = (seq_t)k * k * k;
        if (k3 > sn)
            break;           /* no further k can produce a sum ≤ n */
        /* compute B = ((k-1)k/2)^2 */
        seq_t tprev = (seq_t)(k - 1) * k / 2;
        seq_t Bprev_sq = tprev * tprev;
        seq_t target = sn + Bprev_sq;
        ensure_A_upto(target);
        /* binary search A for value == target */
        size_t lo = 0, hi = A_len;
        while (lo < hi)
        {
            size_t mid = (lo + hi) >> 1;
            seq_t val = A[mid];
            if (val == target)
            {
                size_t m = mid + 1;   /* A[0] corresponds to m=1 */
                if (m >= k)
                    ++res;
                break;
            }
            else if (val < target)
                lo = mid + 1;
            else
                hi = mid;
        }
    }
    return res;
}
