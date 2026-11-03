/*
 * A340897  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(6,n) (with n at least 3) missing two edges, where the two
 *          removed edges are not incident to the same vertex in the 6-point
 *          set and are also not incident to t...
 * https://oeis.org/A340897
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 961*63^(n-2) - 1830*31^(n-2) + 1359*15^(n-2) -
 *            484*7^(n-2) + 79*3^(n-2) - 4 (n >= 3)
 * Range:     u8 none, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 533 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    idx_t exp = n - 2;          /* exponent for all powers */

    seq_t p63 = ipow((seq_t)63, (seq_t)exp);
    seq_t p31 = ipow((seq_t)31, (seq_t)exp);
    seq_t p15 = ipow((seq_t)15, (seq_t)exp);
    seq_t p7  = ipow((seq_t)7,  (seq_t)exp);
    seq_t p3  = ipow((seq_t)3,  (seq_t)exp);

    seq_t result = seq_mul(p63, (seq_t)961);          /* 961*63^(n-2) */

    seq_t tmp = seq_mul(p31, (seq_t)1830);            /* 1830*31^(n-2) */
    result -= tmp;                                   /* subtract */

    tmp = seq_mul(p15, (seq_t)1359);                  /* 1359*15^(n-2) */
    result += tmp;                                   /* add */

    tmp = seq_mul(p7, (seq_t)484);                   /* 484*7^(n-2) */
    result -= tmp;                                   /* subtract */

    tmp = seq_mul(p3, (seq_t)79);                    /* 79*3^(n-2) */
    result += tmp;                                   /* add */

    result -= (seq_t)4;                              /* subtract 4 */

    return result;
}
