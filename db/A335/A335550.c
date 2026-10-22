/*
 * A335550  Number of minimal normal patterns avoided by the prime indices of
 *          n in increasing or decreasing order, counting multiplicity.
 * https://oeis.org/A335550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335550:
   a(1) = 1.
   For n > 1, a(n) = 3 if n is a power of a squarefree number
   (i.e., all prime exponents in its factorisation are equal),
   otherwise a(n) = 4. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle the first term explicitly */
    if (n == 1)
        return 1;
    if (n == 0) /* undefined case – return 0 safely */
        return 0;

    /* Factorise n and check that all exponents are equal */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    bool equal = true;
    if (cnt > 0) {
        uint64_t first = exps[0];
        for (size_t i = 1; i < cnt; ++i) {
            if (exps[i] != first) {
                equal = false;
                break;
            }
        }
    }

    return equal ? 3 : 4;
}
