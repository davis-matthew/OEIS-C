/*
 * A374854  a(n) = (1/30)*A028361(n) for n>=3.
 * https://oeis.org/A374854
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A374854:  a(n) = (1/30) * A028361(n)
   Formula used:  a(3)=1,  a(n+1)=a(n)*(2^n+1)  (for n≥3)
   Offset: 3   (so a(3) is the first term)
   The implementation uses a divide‑and‑conquer product:
   a(n) = ∏_{k=3}^{n-1} (2^k + 1)      */

#include "../utils/pow2.h"

static seq_t prod_range(idx_t l, idx_t r) {
    /* return the product of (2^k + 1) for all k in [l, r] */
    if (l > r) return (seq_t)1;          /* empty product */
    if (l == r) {
        seq_t val = pow2(l);              /* 2^l */
        return val + (seq_t)1;           /* 2^l + 1 */
    }
    idx_t mid = l + (r - l) / 2;         /* split range, log(n) depth */
    seq_t left  = prod_range(l, mid);
    seq_t right = prod_range(mid + 1, r);
    return left * right;
}

seq_t a(idx_t n) {
    if (n < 3) return (seq_t)0;          /* undefined below offset */
    return prod_range((idx_t)3, n - 1);   /* a(n) = Π_{k=3}^{n-1} (2^k+1) */
}
