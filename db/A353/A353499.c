/*
 * A353499  a(n) = 1 if n is a squarefree number for which the 2-adic
 *          valuation of phi(n) does not exceed the 2-adic valuation of n-1,
 *          otherwise 0.
 * https://oeis.org/A353499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is squarefree and v₂(φ(n)) ≤ v₂(n–1), else 0
 * Range:     u8 n <= 349, u16 n <= 9649, u32 n <= 9649, u64 n <= 9649,
 *            u128 n <= 9649, big any n
 * Helpers:   is_squarefree, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1 (offset 1) */
    if (n == 0) return 0;      /* out‑of‑range guard */
    if (n == 1) return 0;      /* a(1) = 0 by definition */

    /* n must be squarefree */
    if (!is_squarefree((seq_t)n)) return 0;

    /* 2‑adic valuation of n–1 */
    idx_t n1   = n - 1;
    int   v2_n = 0;
    while ((n1 & 1) == 0) {
        n1 >>= 1;
        v2_n++;
    }

    /* 2‑adic valuation of φ(n) */
    seq_t phi_n = phi(n);
    seq_t tmp   = phi_n;
    int   v2_phi = 0;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        v2_phi++;
    }

    return (v2_phi <= v2_n) ? 1 : 0;
}
