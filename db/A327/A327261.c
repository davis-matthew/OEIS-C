/*
 * A327261  Numbers > 1 not of the form 2n*k - A319929(n,k) where n and k > 1.
 * https://oeis.org/A327261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer m>1 not of the form 2*i*j - A319929(i,j)
 *            with i,j>1; computed by sieve (marking all such values up to a
 *            doubling limit)
 * Range:     u8 n <= 110, u16 n <= 2858, u32 n <= 2858, u64 n <= 2858,
 *            u128 n <= 2858, big any n
 * Validated: standard, pass, 75 terms through n = 2619 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned char *A327261_mark = 0;
static uint64_t A327261_L = 0;

static uint64_t A327261_val(uint64_t n, uint64_t k)
{
    if ((n & 1) == 0) {
        if ((k & 1) == 0) return 2 * n * k;
        return n * (2 * k - 1);
    } else {
        if ((k & 1) == 0) return k * (2 * n - 1);
        return 2 * n * k - n - k + 1;
    }
}

static void A327261_build(uint64_t L)
{
    uint64_t n, k, v;
    if (A327261_mark) free(A327261_mark);
    A327261_mark = (unsigned char *)calloc((size_t)(L + 1), 1);
    A327261_L = L;
    for (n = 2; ; n++) {
        if (A327261_val(n, 2) > L) break;
        for (k = 2; ; k++) {
            v = A327261_val(n, k);
            if (v > L) break;
            A327261_mark[v] = 1;
        }
    }
}

seq_t a(idx_t n)
{
    for (;;) {
        if (A327261_L == 0) A327261_build((uint64_t)n * 4 + 64);
        uint64_t cnt = 0, m;
        for (m = 2; m <= A327261_L; m++) {
            if (!A327261_mark[m]) {
                cnt++;
                if (cnt == (uint64_t)n) return (seq_t)m;
            }
        }
        A327261_build(A327261_L * 2);
    }
}
