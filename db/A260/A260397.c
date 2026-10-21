/*
 * A260397  Infinite palindromic word (a(1),a(2),a(3),...) with initial word
 *          w(1) = (1,1,0) and midword sequence (a(n)); see Comments.
 * https://oeis.org/A260397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=1, a(3)=0. For n>3: if n is a power of two,
 *            a(n)=a(log2(n)-1); otherwise a(n)=a(2^{⌊log2(n)⌋+1} - n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_log2_idx(idx_t x)
{
    /* x > 0 */
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n <= 3) {
        static const seq_t base[3] = {1, 1, 0};
        return base[n - 1];
    }
    /* Power‑of‑two case */
    if ((n & (n - 1)) == 0) {
        idx_t e = floor_log2_idx(n);      /* e >= 2 since n > 3 */
        return a(e - 1);
    }
    /* Mirror case */
    idx_t p      = floor_log2_idx(n);
    idx_t mirror = (idx_t)1 << (p + 1);  /* 2^(p+1) */
    idx_t mir    = mirror - n;           /*  mir < n */
    return a(mir);
}
