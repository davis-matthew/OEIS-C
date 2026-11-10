/*
 * A379472  Composition of Blue code and bijective bit-reverse, in this order:
 *          a(n) = A057889(A193231(n)).
 * https://oeis.org/A379472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A057889(A193231(n)); A193231 = XOR of (x+1)^e over set
 *            bits e of n (GF(2) polynomial substitution x->x+1); A057889 =
 *            reverse binary digits of odd part, keep trailing zeros
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 96 terms through n = 9975 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A193231_a(idx_t n)
{
    seq_t res  = 0;
    seq_t mask = 1;
    idx_t t = n;

    while (t) {
        if (t & 1)
            res ^= mask;
        mask = (mask << 1) ^ mask;
        t >>= 1;
    }
    return res;
}

static seq_t A057889_a(seq_t v)
{
    if (v == 0) return 0;

    unsigned tz = 0;
    while ((v & 1) == 0) {
        v >>= 1;
        ++tz;
    }

    /* v is odd now; reverse its bits */
    seq_t rev = 0;
    while (v != 0) {
        rev = (rev << 1) | (v & 1);
        v >>= 1;
    }
    return rev << tz;
}

seq_t a(idx_t n)
{
    return A057889_a(A193231_a(n));
}
