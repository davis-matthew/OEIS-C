/*
 * A350531  Triangle read by rows: T(n,k) is the number of labeled
 *          loop-threshold graphs on vertex set [n] with k components, for n
 *          >= 1 and 1 <= k <= n.
 * https://oeis.org/A350531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,n) = n+1; T(1,1) = 2; T(n,1) = Sum_{j=0..n-1}
 *            A173018(n,j)*2^j for n >= 2; T(n,k) = binomial(n,k-1)*T(n-k+1,1)
 *            for 2 <= k <= n-1. Triangle read by rows
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-10)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/binomial.h"

/* T(m,1): Eulerian row m weighted by powers of 2 */
static seq_t lt_first(idx_t m)
{
    if (m == 1) return (seq_t)2;
    seq_t *e = mem_new(seq_t, (size_t)m + 2);
    e[0] = (seq_t)1;
    for (idx_t i = 1; i <= m; ++i) {
        for (idx_t k = i - 1; k >= 1; --k) {
            e[k] = (seq_t)(k + 1) * e[k] + (seq_t)(i - k) * e[k - 1];
        }
        e[0] = (seq_t)1;
        e[i] = (seq_t)0;
    }
    seq_t s = (seq_t)0;
    for (idx_t j = m; j-- > 0;) {
        s = s * 2 + e[j];
    }
    mem_free(e);
    return s;
}

seq_t a(idx_t n)
{
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;
    idx_t k = n - r * (r - 1) / 2;   /* 1..r */

    if (k == r) return (seq_t)r + 1;
    if (k == 1) return lt_first(r);
    return binomial(r, k - 1) * lt_first(r - k + 1);
}
