/*
 * A246039  Number of odd terms in f^n, where f = (1/x+1+x)*(1/y+y)+1.
 * https://oeis.org/A246039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Run Length Transform of A246038: a(n) = Product_i S(i)
 *            over lengths i of runs of 1's in binary n, with S = A246038:
 *            S(0)=1, S(1)=7, S(2)=29, S(3)=103,
 *            S(i)=3*S(i-1)+8*S(i-3)+8*S(i-4) for i>=4
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 80 terms through n = 7767 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A246038_s(seq_t *c, int i) {
    seq_t val = 0;
    val += (seq_t)3 * c[i - 1];
    if (i >= 3) val += (seq_t)8 * c[i - 3];
    if (i >= 4) val += (seq_t)8 * c[i - 4];
    if (i == 1) val += 4;
    else if (i == 2 || i == 3) val += 8;
    return val;
}

seq_t a(idx_t n) {
    seq_t *S = mem_new(seq_t, 66);
    S[0] = 1;
    for (int i = 1; i <= 65; ++i) S[i] = A246038_s(S, i);

    seq_t result = 1;
    int run = 0;
    for (int b = 0; b < 64; ++b) {
        if ((n >> b) & 1) {
            ++run;
        } else {
            if (run > 0) result = result * S[run];
            run = 0;
        }
    }
    if (run > 0) result = result * S[run];
    mem_free(S);
    return result;
}
