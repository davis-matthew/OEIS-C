/*
 * A386376  a(n) is the smallest integer k such that b(n,k) is squarefree,
 *          where b(n,1) = n and b(n, k+1) = b(n,k) + rad(b(n, k)) for k >= 1.
 * https://oeis.org/A386376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimal k such that b(n,1) = n, and b(n,k+1) = b(n,k) +
 *            rad(b(n,k)) is squarefree. That is, a(n) = min{k ≥ 1 : b(n,k)
 *            has no squared prime factor}
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_squarefree, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Needed utilities */
#include <stdbool.h>
#include "../utils/rad.h"
#include "../utils/is_squarefree.h"

/* Compute A386376(n) */
seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined for non‑positive n; return 0 */
        return 0;

    seq_t cur   = (seq_t)n;   /* current value of b(n,k) */
    seq_t k     = 1;          /* k starts at 1 */

    /* iterate until cur becomes squarefree */
    while (!is_squarefree(cur)) {
        /* rad is defined for idx_t (uint64_t); cur must fit in it */
        seq_t r = rad((idx_t)cur);   /* distinct prime product of cur */
        cur += r;                    /* b(n,k+1) = cur + rad(cur) */
        ++k;
    }

    return k;   /* minimal k for which b(n,k) is squarefree */
}
