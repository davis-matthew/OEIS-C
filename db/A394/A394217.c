/*
 * A394217  Earliest monotonic sequence satisfying a(1)=1 and a(a(n)+a(n-1))=n
 *          for n>=1 (with a(k)=0 for k<=0).
 * https://oeis.org/A394217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(2)=2; a(3)=2. For n≥4, write n = 2^k + r with 0 ≤ r <
 *            2^k, k ≥ 2. If r ≤ 2^(k‑1) then a(n) = 3·2^(k‑2) + ceil(r/2);
 *            otherwise a(n) = n – 2^(k‑1)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t floor_log2(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(k)=0 for k≤0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)2;

    /* n ≥ 4 */
    idx_t k = floor_log2(n);          /* highest power of 2 ≤ n */
    idx_t pow2 = ((idx_t)1) << k;     /* 2^k */
    idx_t r = n - pow2;               /* remainder r */
    idx_t half = pow2 >> 1;           /* 2^(k-1) */

    if (r <= half) {
        /* a(n) = 3*2^(k-2) + ceil(r/2) */
        idx_t two_k_minus_two = half >> 1;       /* 2^(k-2) */
        seq_t term = (seq_t)3 * (seq_t)two_k_minus_two
                     + (seq_t)((r + 1) >> 1);   /* ceil(r/2) = (r+1)/2 */
        return term;
    } else {
        /* a(n) = n - 2^(k-1) */
        return (seq_t)(n - half);
    }
}
