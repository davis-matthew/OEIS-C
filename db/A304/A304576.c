/*
 * A304576  a(n) = Sum_{k = 1..n, k squarefree and relatively prime to n}
 *          (-1)^(k-1).
 * https://oeis.org/A304576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Σ_{k = 1..n, k squarefree, gcd(k,n)=1} (-1)^(k-1)
 * Range:     u8 n <= 535, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;
    seq_t sn = (seq_t)n;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* gcd(k,n) == 1  &&  k is square‑free */
        if (gcd((seq_t)k, sn) != (seq_t)1) continue;
        if (!is_squarefree((seq_t)k)) continue;
        /* add or subtract 1 according to the parity of k */
        if (k & 1U)
            sum += (seq_t)1;   /* odd k → +1 */
        else
            sum -= (seq_t)1;   /* even k → –1 */
    }
    return sum;
}
