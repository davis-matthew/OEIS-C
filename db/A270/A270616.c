/*
 * A270616  Number of ordered ways to write n as the sum of a positive square,
 *          the square of a triangular number, and a generalized pentagonal
 *          number (A001318).
 * https://oeis.org/A270616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A270616
 * a(n) = number of ordered triples (u,k,m) with
 *  u ≥ 1, k ≥ 0, m ∈ ℤ such that
 *      n = u² + (k(k+1)/2)² + m(3m-1)/2
 * where m(3m-1)/2 runs over the generalized pentagonal numbers (including 0).
 * The function enumerates all possibilities for a given n.
 */

#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for the offset 1 sequence */

    seq_t count = 0;

    /* iterate over positive squares u² ≤ n */
    for (idx_t u = 1; ; ++u) {
        idx_t su = u * u;
        if (su > n)
            break;

        idx_t rem_u = n - su;

        /* iterate over squares of triangular numbers t_k² ≤ rem_u */
        for (idx_t k = 0; ; ++k) {
            idx_t t   = (k * (k + 1)) / 2;   /* triangular number */
            idx_t qt  = t * t;                /* its square */
            if (qt > rem_u)
                break;

            idx_t r = rem_u - qt;            /* candidate pentagonal part */

            /* check if r is a generalized pentagonal number */
            idx_t D     = 1 + 24 * r;
            idx_t sqrtD = _floor_isqrt_u64(D);
            if (sqrtD * sqrtD == D) {
                /* sqrtD ≡ 1 (mod 6)  or  sqrtD ≡ 5 (mod 6)  ⇔ r is pentagonal */
                if ((sqrtD % 6 == 1) || (sqrtD % 6 == 5))
                    count += 1;
            }
        }
    }

    return count;
}
