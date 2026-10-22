/*
 * A341775  Number of partitions of n into 4 nonzero tetrahedral numbers.
 * https://oeis.org/A341775
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 4 nonzero tetrahedral
 *            numbers. Tetrahedral numbers: T(k) = k(k+1)(k+2)/6 (k ≥ 1)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* maximum number of tetrahedral numbers needed for n ≤ 10000 */
#define MAX_TETRA  80

seq_t a(idx_t n)
{
    /* sequence is defined for n ≥ 4; below we return 0 */
    if (n < 4)
        return 0;

    /* Pre‑compute all tetrahedral numbers ≤ n */
    seq_t tetra[MAX_TETRA];
    size_t m = 0;                          /* number of tetrahedral numbers found */
    for (idx_t k = 1; ; ++k)
    {
        seq_t t = (seq_t)k * (k + 1) * (k + 2) / 6;  /* T(k) */
        if (t > n || m == MAX_TETRA)
            break;
        tetra[m++] = t;
    }

    /* Count unordered quadruples (i ≤ j ≤ k ≤ l) with sum == n */
    seq_t count = 0;
    for (size_t i = 0; i < m; ++i)
    {
        seq_t ti = tetra[i];
        if (ti * 4 > n)  /* smallest possible sum with this i is too big */
            break;
        for (size_t j = i; j < m; ++j)
        {
            seq_t tj = tetra[j];
            if (ti + tj * 3 > n)
                break;
            for (size_t k = j; k < m; ++k)
            {
                seq_t tk = tetra[k];
                if (ti + tj + tk * 2 > n)
                    break;
                for (size_t l = k; l < m; ++l)
                {
                    seq_t tl = tetra[l];
                    seq_t sum = ti + tj + tk + tl;
                    if (sum < n)
                        continue;
                    if (sum == n)
                        ++count;
                    else            /* sum > n, further l will only increase sum */
                        break;
                }
            }
        }
    }
    return count;
}
