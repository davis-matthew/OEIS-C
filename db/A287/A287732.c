/*
 * A287732  Bisection of A287730.
 * https://oeis.org/A287732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(1)=0. For n>1 let t=floor_log2(n‑1); let parity=t mod 2.
 *            Initialize s=c=1. For each i=0..t-1 (i scans the bits of n‑1
 *            from least significant to most, excluding the leading 1): bit =
 *            (n‑1)>>i & 1 if bit == parity : s += c else : c += s Then
 *            a(n)=s. This gives the same sequence as A287732
 * Range:     u8 n <= 4393, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static inline int log2_floor_uint64(idx_t v) {
    int r = 0;
    while (v >>= 1) ++r;
    return r;
}

seq_t a(idx_t n) {
    if (n == 1) return 0;

    seq_t s = (seq_t)1;
    seq_t c = (seq_t)1;
    idx_t x = n - 1;

    int t = log2_floor_uint64(x);      /* number of bits minus one   */
    int parity = t & 1;                /* t mod 2                    */

    for (idx_t i = 0; i < (idx_t)t; ++i) {
        int bit = (x >> i) & 1;
        if (bit == parity)
            s += c;
        else
            c += s;
    }

    return s;
}
