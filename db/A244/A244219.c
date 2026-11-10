/*
 * A244219  The starting points of runs of consecutive terms with the same
 *          parity in A014418 (n represented in Greedy Catalan Base).
 * https://oeis.org/A244219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = n-th position m >= 1 where A014418(m-1) and
 *            A014418(m) differ in parity (found by scanning m upward)
 * Range:     u8 n <= 217, u16 n <= 4121, u32 n <= 4121, u64 n <= 4121,
 *            u128 n <= 4121, big any n
 * Validated: standard, pass, 86 terms through n = 3862 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A244219_cat[32];
static int A244219_catn = 0;

static int A244219_par(uint64_t m)
{
    if (A244219_catn == 0) {
        A244219_cat[0] = 1;
        for (int k = 0; k < 30; ++k)
            A244219_cat[k + 1] = A244219_cat[k] * (uint64_t)(2 * (2 * k + 1)) / (uint64_t)(k + 2);
        A244219_catn = 31;
    }
    if (m == 0) return 0;
    int mk = 0;
    for (int k = 1; k < A244219_catn; ++k) {
        if (A244219_cat[k] > m) break;
        mk = k;
    }
    uint64_t rem = m;
    uint64_t last = 0;
    for (int k = mk; k >= 1; --k) {
        uint64_t c = A244219_cat[k];
        uint64_t d = rem / c;
        if (d > 3) d = 3;
        rem -= d * c;
        last = d;
    }
    return (int)(last & 1);
}

seq_t a(idx_t n)
{
    static uint64_t *pos = NULL;
    static uint64_t cnt = 0, cap = 0;
    static uint64_t scanned = 0;
    static int prevpar = 0;

    if (!pos) {
        cap = 1024;
        pos = (uint64_t *)malloc(cap * sizeof(uint64_t));
        pos[0] = 0;
        cnt = 1;
    }
    while (cnt <= (uint64_t)n) {
        uint64_t m = scanned + 1;
        int p = A244219_par(m);
        if (p != prevpar) {
            if (cnt >= cap) {
                cap *= 2;
                pos = (uint64_t *)realloc(pos, cap * sizeof(uint64_t));
            }
            pos[cnt++] = m;
        }
        prevpar = p;
        scanned = m;
    }
    return (seq_t)pos[n];
}
