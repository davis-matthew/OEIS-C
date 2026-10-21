/*
 * A262064  Expansion of f(x^9, x^15) / f(-x^2, -x^4) in powers of x where f(,
 *          ) is the Ramanujan general theta function.
 * https://oeis.org/A262064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 87, u32 n <= 253, u64 n <= 820,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes a(n) as the Euler transform of the
   period‑48 sequence  (part_allowed).  The value a(n) is the number
   of partitions of n into parts j such that part_allowed[(j−1) mod 48]
   = 1.  The algorithm memoises the partition table up to the maximum
   n requested. */

#include <stdlib.h>

static const unsigned char part_allowed[48] = {
    0,1,0,1,0,1,0,1,
    1,1,0,1,0,1,1,1,
    0,0,0,1,0,1,0,0,
    0,1,0,1,0,0,0,1,
    1,1,0,1,0,1,1,1,
    0,1,0,1,0,1,0,0
};

static seq_t *dp        = NULL;   /* cached DP array            */
static idx_t dp_size    = 0;     /* number of computed entries */

seq_t a(idx_t n)
{
    /* Grow the DP array if a larger n is requested.  The allocation
       strategy differs between C and C++ so that constructors/destructors
       of an arbitrary‐precision seq_t are correctly handled. */
    if (n >= dp_size) {
        idx_t   size   = n + 1;
        seq_t * newdp;

#ifdef __cplusplus
        newdp = new seq_t[size];               /* C++ allocation          */
#else
        newdp = (seq_t *)malloc(sizeof(seq_t) * size);
#endif
        if (!newdp) return (seq_t)0;           /* allocation failure      */

        /* initialise to zero */
        for (idx_t i = 0; i < size; ++i) newdp[i] = (seq_t)0;
        newdp[0] = (seq_t)1;                   /* empty partition         */

        /* DP: for each allowed part j, add its contribution. */
        for (idx_t j = 1; j <= n; ++j) {
            if (part_allowed[(j - 1) % 48]) {
                for (idx_t k = j; k <= n; ++k) {
                    newdp[k] += newdp[k - j];
                }
            }
        }

        /* free old table */
        if (dp != NULL) {
#ifdef __cplusplus
            delete[] dp;
#else
            free(dp);
#endif
        }

        dp      = newdp;
        dp_size = size;
    }

    return dp[n];
}
