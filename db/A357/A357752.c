/*
 * A357752  a(n) is the largest perfect power < 2^n.
 * https://oeis.org/A357752
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 35 (2026-11-01)
 * Helpers:   bit_length, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = largest perfect power < 2^n   (offset 3, n ≥ 3)

   The algorithm iterates over all admissible exponents e (2 ≤ e ≤ n‑1).
   For each e it computes the integer e‑th root of the limit L = 2^n
   by binary search in a very small interval: the root lies between
   2^⌊log₂(L)/e⌋ and 2^⌊log₂(L)/e⌋+2.
   Using seq_pow for exponentiation by squaring keeps the logarithmic
   cost in the exponent e.  The largest candidate over all e is returned.
   This method works for the arbitrary‑precision build and, for 64‑bit
   builds, for all n whose answer fits in seq_t.
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bit_length.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n < 3) {                       /* undefined for n < 3 */
        return (seq_t)0;
    }

    /* 2^n computed by exponentiation to avoid overflow of shift operators */
    const seq_t limit = ipow_idx((seq_t)2, n);   /* L = 2^n */

    seq_t best = (seq_t)0;

    /* pre‑compute the bit length of the limit once */
    const idx_t limit_bitlen = bit_length(limit);   /* = n + 1 for exact powers of 2 */

    /* scan exponents 2 … n‑1 (e = n would give base = 1) */
    for (idx_t e = 2; e <= n - 1; ++e) {

        /* bounds for the integer root: 2^lowbits ≤ root < 2^highbits   */
        const idx_t lowbits  = (limit_bitlen - 1) / e;   /* floor(log₂(L)/e) */
        const idx_t highbits = lowbits + 2;              /* guaranteed upper bound */

        seq_t low  = ipow_idx((seq_t)2, lowbits);  /* 2^lowbits  */
        seq_t high = ipow_idx((seq_t)2, highbits); /* 2^highbits */

        /* binary search for largest b with b^e < limit */
        seq_t lo = low;
        seq_t hi = high;
        while (lo < hi) {
            seq_t mid = lo + ((hi - lo) >> 1);          /* mid = floor((lo+hi)/2) */
            seq_t mid_pw = ipow_idx(mid, e);
            if (mid_pw < limit)
                lo = mid + 1;
            else
                hi = mid;
        }

        seq_t base = lo - 1;                /* lo is first ≥ limit */
        if (base < (seq_t)2)                /* discard base < 2 */
            continue;

        seq_t val = ipow_idx(base, e);
        if (val > best)
            best = val;
    }

    return best;
}
