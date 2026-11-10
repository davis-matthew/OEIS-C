/*
 * A341258  Concatenation of all 01-words, in the order induced by A000201;
 *          see Comments.
 * https://oeis.org/A341258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   concatenate words w(k), k>=1, where w(1)=0, w(2)=1,
 *            w(k)=0w(s'(k)) if k in A000201, w(k)=1w(t'(k)) if k in A001950;
 *            a(n)=n-th digit
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-10)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128.h"

/* lower Wythoff L(k) = floor(k*phi) */
static uint64_t A341258_L(uint64_t k)
{
    unsigned __int128 K = (unsigned __int128)k;
    unsigned __int128 r = isqrt_u128(5 * K * K);
    return (uint64_t)((K + r) / 2);
}

/* returns 1 if k is in the upper Wythoff sequence, 0 if in the lower */
static int A341258_bit(uint64_t k)
{
    return (A341258_L(k + 1) - A341258_L(k) == 2) ? 0 : 1;
}

/* parent index for k >= 3 */
static uint64_t A341258_parent(uint64_t k)
{
    uint64_t L = A341258_L(k);
    if (A341258_bit(k) == 0) return L - k;
    return 2 * k - 1 - L;
}

static uint64_t A341258_len(uint64_t k)
{
    uint64_t c = 1;
    while (k > 2) {
        k = A341258_parent(k);
        c++;
    }
    return c;
}

seq_t a(idx_t n)
{
    uint64_t k = 1;
    uint64_t cum = 0;
    uint64_t target = (uint64_t)n;
    uint64_t len;
    for (;;) {
        len = A341258_len(k);
        if (cum + len >= target) break;
        cum += len;
        k++;
    }
    uint64_t p = target - cum; /* 1-indexed position inside w(k) */
    while (p > 1) {
        p--;
        k = A341258_parent(k);
    }
    if (k == 1) return (seq_t)0;
    if (k == 2) return (seq_t)1;
    return (seq_t)A341258_bit(k);
}
