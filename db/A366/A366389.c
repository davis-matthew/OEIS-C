/*
 * A366389  Variant of bijective bit-reverse of n (A057889) for which it holds
 *          that a(n) == n (mod 3).
 * https://oeis.org/A366389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   if A057889(n) == n (mod 3) then a(n) = A057889(n), else a(n) =
 *            A073675(A057889(n)); a(0) = 0
 * Range:     u8 n <= 130, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 96 terms through n = 8417 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A366389_rev(idx_t n)
{
    unsigned tz = 0;
    idx_t m = n;
    while ((m & 1) == 0) { m >>= 1; ++tz; }
    seq_t rev = 0;
    while (m) {
        rev = (rev << 1) | (seq_t)(m & 1);
        m >>= 1;
    }
    return rev << tz;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t r = A366389_rev(n);
    unsigned rm = (unsigned)(r % 3);
    unsigned nm = (unsigned)(n % 3);
    if (rm == nm) return r;
    /* A073675 applied to r: 2r if v2(r) even, else r/2 */
    seq_t t = r;
    int v = 0;
    while ((t & 1) == 0) { t >>= 1; ++v; }
    if ((v & 1) == 0) return r * 2;
    return r >> 1;
}
