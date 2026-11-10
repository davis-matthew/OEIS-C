/*
 * A356087  Intersection of A001952 and A054406.
 * https://oeis.org/A356087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer lying in both Beatty sequences
 *            floor(k*(2+sqrt(2))) (A001952) and floor(k*(3+sqrt(3))/2)
 *            (A054406); found by a two-pointer merge
 * Range:     u8 none, u16 n <= 44, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-10)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

static seq_t A356087_b1(idx_t k)
{
    seq_t ks = (seq_t)k;
    return ks + ks + isqrt_seq((seq_t)2 * ks * ks);
}

static seq_t A356087_b2(idx_t k)
{
    seq_t ks = (seq_t)k;
    seq_t s = isqrt_seq((seq_t)3 * ks * ks);
    seq_t t = (seq_t)3 * ks + s;
    return t >> 1;
}

seq_t a(idx_t n)
{
    idx_t i = 1, j = 1;
    seq_t vi = A356087_b1(i);
    seq_t vj = A356087_b2(j);
    idx_t found = 0;
    for (;;) {
        if (vi == vj) {
            ++found;
            if (found == n)
                return vi;
            ++i; vi = A356087_b1(i);
            ++j; vj = A356087_b2(j);
        } else if (vi < vj) {
            ++i; vi = A356087_b1(i);
        } else {
            ++j; vj = A356087_b2(j);
        }
    }
}
