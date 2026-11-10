/*
 * A356086  Intersection of A001952 and A022838.
 * https://oeis.org/A356086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th common term of A001952(k)=floor(k*(2+sqrt(2))) and
 *            A022838(k)=floor(k*sqrt(3)), found by a two-pointer merge
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

static seq_t A356086_u(seq_t k)
{
    return (seq_t)2 * k + isqrt_seq((seq_t)2 * k * k);
}

static seq_t A356086_v(seq_t k)
{
    return isqrt_seq((seq_t)3 * k * k);
}

seq_t a(idx_t n)
{
    seq_t i = 1, j = 1;
    seq_t vi = A356086_u(i), vj = A356086_v(j);
    idx_t count = 0;
    for (;;) {
        if (vi == vj) {
            count++;
            if (count == n) return vi;
            i++; j++;
            vi = A356086_u(i);
            vj = A356086_v(j);
        } else if (vi < vj) {
            i++;
            vi = A356086_u(i);
        } else {
            j++;
            vj = A356086_v(j);
        }
    }
}
