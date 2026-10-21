/*
 * A254767  a(n) is the least k > n such that k*n is a cube.
 * https://oeis.org/A254767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the least k > n such that n*k is a perfect cube
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* Factor n. */
    uint64_t primes[64], exps[64];
    size_t pcnt;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Compute the minimal multiplier c that makes n*c a cube. */
    seq_t c = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t e = exps[i] % 3;
        uint64_t need = (3 - e) % 3;             /* 0, 1 or 2 */
        if (need) {
            seq_t p = (seq_t)primes[i];
            if (need == 2) {
                c *= p * p;    /* p² may overflow uint64_t but is modulo‐defined. */
            } else {            /* need == 1 */
                c *= p;
            }
        }
    }

    /* Find the smallest integer t such that c * t³ > n. */
    seq_t temp = ((seq_t)n) / c;          /* floor of n/c, 0 if n < c. */
    seq_t t    = floor_cube_root(temp);   /* largest t with c*t³ ≤ n. */
    seq_t next = t + (seq_t)1;            /* smallest t with c*t³ > n. */
    seq_t next_cube = next * next * next;
    seq_t k = c * next_cube;              /* a(n) = k. */

    return k;
}
