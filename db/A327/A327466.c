/*
 * A327466  Number of nonempty subsets of [1..n] which are geometric
 *          progressions with rational ratio and are locally maximal.
 * https://oeis.org/A327466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the number of locally‑maximal geometric progressions
 *            (strictly increasing, rational ratio) in [1..n]. The
 *            implementation counts all GPs that cannot be extended with the
 *            same ratio and whose ratio is not a perfect power (otherwise
 *            they can be extended by a smaller ratio). This yields the c...
 * Range:     u8 n <= 24, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute gcd for idx_t (uint64_t). */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 1;                 /* a(1)=1, a(2)=1 handled in loops below */

    /* Pre‑compute perfect‑power masks up to n.  
       powerMask[v] has bit e set if v is a perfect e‑th power (e>=2).   */
    uint16_t *powerMask = (uint16_t*)malloc((n + 1) * sizeof(uint16_t));
    if (!powerMask) return 0;     /* unlikely, but safe */
    for (idx_t i = 0; i <= n; ++i) powerMask[i] = 0;

    /* set all exponents for value 1 (1^e == 1 for any e>1) */
    uint16_t allOnes = 0x7FFF;   /* bits 1..14 set */
    if (n >= 1) powerMask[1] = allOnes;

    /* populate power masks */
    for (idx_t e = 2; (1ULL << e) <= n; ++e) {
        for (idx_t base = 2; ; ++base) {
            /* compute base^e safely */
            unsigned long long val = 1;
            for (idx_t k = 0; k < e; ++k) {
                val *= base;
                if (val > n) break;
            }
            if (val > n) break;
            powerMask[(idx_t)val] |= (uint16_t)(1u << e);
        }
    }

    seq_t ans = 0;
    for (idx_t a_val = 2; a_val <= n; ++a_val) {
        for (idx_t b_val = 1; b_val < a_val; ++b_val) {
            if (gcd_idx(a_val, b_val) != 1) continue;

            /* skip ratios that are a perfect power */
            if ( (powerMask[a_val] & powerMask[b_val]) >> 2 )
                continue;            /* some e>=2 in common → perfect power */

            idx_t aPowLminus1 = a_val;   /* a^(L-1) for L=2 */
            for (idx_t L = 2; aPowLminus1 <= n; ++L) {
                idx_t K1 = n / aPowLminus1;

                idx_t aDivK = K1 / a_val;

                /* compute a^L and T1 = floor(n / a^L) */
                idx_t aPowL = aPowLminus1 * a_val;
                idx_t T1 = aPowL == 0 ? 0 : n / aPowL;

                idx_t M1 = K1 / b_val;
                idx_t M = (M1 < T1) ? M1 : T1;
                idx_t badForward = M - (M / a_val);

                idx_t good = K1 - aDivK - badForward;
                if (good)
                    ans += (seq_t)good;

                /* prepare next L */
                if (aPowLminus1 > n / a_val) break;
                aPowLminus1 *= a_val;
            }
        }
    }

    free(powerMask);
    return ans;
}
