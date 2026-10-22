/*
 * A364172  a(n) = (6*n)!*(n/3)!/((3*n)!*(2*n)!*(4*n/3)!).
 * https://oeis.org/A364172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5832·∏(6n−odd)/∏(denoms)·a(n−3); a(0)=1, a(1)=45,
 *            a(2)=6237 where odd = 1,5,7,11,13,17 denoms = n, n−1, n−2, 2n−3,
 *            4n−3, 4n−9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        /* grow cache to hold indices 0..n */
        size_t new_size = (size_t)(n + 1);
        seq_t *larger = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!larger) return (seq_t)0;      /* out of memory – unlikely in tests */

        /* initialise new entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                larger[i] = (seq_t)1;
            } else if (i == 1) {
                larger[i] = (seq_t)45;
            } else if (i == 2) {
                larger[i] = (seq_t)6237;
            } else {
                /* recurrence for i >= 3 */
                seq_t val = larger[i - 3];

                /* denominator factors */
                seq_t den[6];
                den[0] = (seq_t)i;
                den[1] = (seq_t)(i - 1);
                den[2] = (seq_t)(i - 2);
                den[3] = (seq_t)(2 * i - 3);
                den[4] = (seq_t)(4 * i - 3);
                den[5] = (seq_t)(4 * i - 9);

                /* cancel common factors with val */
                for (int d = 0; d < 6; ++d) {
                    seq_t g = gcd(val, den[d]);
                    if (g > 1) {
                        val /= g;
                        den[d] /= g;
                    }
                }

                /* numerator factors */
                seq_t num[7];
                num[0] = (seq_t)5832;
                num[1] = (seq_t)(6 * i - 1);
                num[2] = (seq_t)(6 * i - 5);
                num[3] = (seq_t)(6 * i - 7);
                num[4] = (seq_t)(6 * i - 11);
                num[5] = (seq_t)(6 * i - 13);
                num[6] = (seq_t)(6 * i - 17);

                /* multiply by numerators, cancelling remaining denominators */
                for (int nfi = 0; nfi < 7; ++nfi) {
                    seq_t p = num[nfi];
                    for (int d = 0; d < 6; ++d) {
                        seq_t g = gcd(p, den[d]);
                        if (g > 1) {
                            p /= g;
                            den[d] /= g;
                        }
                    }
                    val *= p;
                }

                /* any remaining denominators should divide val exactly */
                for (int d = 0; d < 6; ++d)
                    if (den[d] > 1)
                        val /= den[d];

                larger[i] = val;
            }
        }

        /* update cache */
        cache = larger;
        cache_size = new_size;
    }

    return cache[n];
}
