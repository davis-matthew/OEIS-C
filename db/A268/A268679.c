/*
 * A268679  a(n) = A268389(A001969(1+n)); A268389 without its zero terms.
 * https://oeis.org/A268679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A268389(A001969(1+n)); A001969(1+n) = 2n+1 if popcount(n)
 *            is odd, else 2n; A268389(m) = largest k with (X+1)^k dividing
 *            the GF(2)[X] polynomial encoded by m
 * Range:     u8 n <= 666, u16 n <= 32767, u32 n <= 32768, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 136 terms through n = 8827 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A268389_a(seq_t cur)
{
    seq_t cnt = 0;
    for (;;) {
        seq_t x = cur;
        int parity = 0;
        while (x) {
            parity ^= (int)(x & 1);
            x >>= 1;
        }
        if (parity || cur == 0)
            break;

        idx_t bits = 0;
        seq_t tmp = cur;
        while (tmp) {
            ++bits;
            tmp >>= 1;
        }

        seq_t next = 0;
        seq_t cum = 0;
        for (idx_t i = 0; i + 1 < bits; ++i) {
            seq_t bit = (cur >> i) & (seq_t)1;
            cum ^= bit;
            next |= cum << i;
        }
        cur = next;
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    int pc = __builtin_popcountll((unsigned long long)n);
    seq_t v = (seq_t)n * 2 + ((pc & 1) ? 1 : 0);
    return A268389_a(v);
}
