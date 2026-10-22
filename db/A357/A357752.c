/*
 * A357752  a(n) is the largest perfect power < 2^n.
 * https://oeis.org/A357752
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   bitlen_seq_t, seq_pow
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

#include "../utils/seq_pow.h"          /* seq_pow, pow2 (via seq_pow) */
#include "../utils/bitlen_seq_t.h"     /* bitlen_seq_t */

seq_t a(idx_t n)
{
    if (n < 3) {                       /* undefined for n < 3 */
        return (seq_t)0;
    }

    /* 2^n computed by exponentiation to avoid overflow of shift operators */
    const seq_t limit = seq_pow((seq_t)2, n);   /* L = 2^n */

    seq_t best = (seq_t)0;

    /* pre‑compute the bit length of the limit once */
    const idx_t limit_bitlen = bitlen_seq_t(limit);   /* = n + 1 for exact powers of 2 */

    /* scan exponents 2 … n‑1 (e = n would give base = 1) */
    for (idx_t e = 2; e <= n - 1; ++e) {

        /* bounds for the integer root: 2^lowbits ≤ root < 2^highbits   */
        const idx_t lowbits  = (limit_bitlen - 1) / e;   /* floor(log₂(L)/e) */
        const idx_t highbits = lowbits + 2;              /* guaranteed upper bound */

        seq_t low  = seq_pow((seq_t)2, lowbits);  /* 2^lowbits  */
        seq_t high = seq_pow((seq_t)2, highbits); /* 2^highbits */

        /* binary search for largest b with b^e < limit */
        seq_t lo = low;
        seq_t hi = high;
        while (lo < hi) {
            seq_t mid = lo + ((hi - lo) >> 1);          /* mid = floor((lo+hi)/2) */
            seq_t mid_pw = seq_pow(mid, e);
            if (mid_pw < limit)
                lo = mid + 1;
            else
                hi = mid;
        }

        seq_t base = lo - 1;                /* lo is first ≥ limit */
        if (base < (seq_t)2)                /* discard base < 2 */
            continue;

        seq_t val = seq_pow(base, e);
        if (val > best)
            best = val;
    }

    return best;
}
