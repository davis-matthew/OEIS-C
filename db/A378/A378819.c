/*
 * A378819  a(n) is the number of distinct nondegenerate triangles whose sides
 *          are prime factors of n.
 * https://oeis.org/A378819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct nondegenerate triangles with side
 *            lengths taken from the set of distinct prime factors of n
 *            (repetitions allowed)
 * Range:     u8 n <= 5262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)              /* n = 1 has no prime factors */
        return 0;

    /* factorial_u64 needs arrays for primes and exponents. */
    size_t     bufsize = 64;                /* enough for any 64‑bit n */
    uint64_t   *pr     = (uint64_t*)malloc(bufsize * sizeof(uint64_t));
    uint64_t   *ex     = (uint64_t*)malloc(bufsize * sizeof(uint64_t));
    size_t     cnt    = 0;

    factor_u64((uint64_t)n, pr, ex, &cnt);  /* cnt = number of distinct primes */

    if (cnt == 0) {              /* n = 1 */
        free(pr); free(ex);
        return 0;
    }
    if (cnt == 1) {              /* prime powers */
        free(pr); free(ex);
        return 1;
    }

    /* Count triples (i,j,k), i<=j<=k, with pr[i]+pr[j] > pr[k]    */
    seq_t ans = 0;
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i; j < cnt; ++j) {
            seq_t sum = (seq_t)pr[i] + (seq_t)pr[j];
            for (size_t k = j; k < cnt; ++k) {
                if (sum <= (seq_t)pr[k])   /* triangle inequality fails */
                    break;
                ++ans;
            }
        }
    }

    free(pr); free(ex);
    return ans;
}
