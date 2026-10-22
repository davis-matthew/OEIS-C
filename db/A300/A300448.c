/*
 * A300448  Number of the integers 4^k*m with k >= 0 and m = 1, 2, 5 such that
 *          n^2 - 4^k*m can be written as the sum of two squares.
 * https://oeis.org/A300448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search: a(n) counts the pairs (k,m) with k>=0 and
 * m∈{1,2,5} such that n^2‑4^k*m is an integer that can be written
 * as the sum of two squares. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sos.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* defined only for n>=1, but safe */
    seq_t count = 0;
    uint64_t nsq = (uint64_t)n * (uint64_t)n;       /* n^2 fits in 64 bits for n<=10000 */
    uint64_t pow4 = 1;                                /* 4^k */
    const uint64_t ms[3] = {1ULL, 2ULL, 5ULL};

    while (true) {
        bool any = false;
        for (int i = 0; i < 3; ++i) {
            uint64_t m = ms[i];
            uint64_t prod = pow4 * m;
            if (prod > nsq) continue;
            any = true;
            uint64_t N = nsq - prod;
            if (sos((int64_t)N)) ++count;
        }
        if (!any) break;               /* no further k will work for any m */
        pow4 <<= 2;                    /* multiply by 4 */
        /* pow4 may overflow if n is huge, but for n<=10000 safe */
    }
    return count;
}
