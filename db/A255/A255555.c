/*
 * A255555  Square array A(row,col) read by downwards antidiagonals: A(1,1) =
 *          1, A(row,1) = A055938(row-1), and for col > 1, A(row,col) =
 *          A005187(1+A(row,col-1)).
 * https://oeis.org/A255555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(1,1)=1, A(row,1)=A055938(row-1),
 *            A(row,col)=A005187(1+A(row,col-1)); read by antidiagonals
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 2080,
 *            u128 n <= 8256, big any n
 * Validated: standard, pass, 94 terms through n = 9899 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static seq_t A005187_f(seq_t n)
{
    seq_t result = 0;
    seq_t p = n;
    while (p > 0) {
        result += p;
        p >>= 1;
    }
    return result;
}

/* j-th integer (j>=1) not of the form A005187(m) */
static uint64_t A055938_f(uint64_t j)
{
    uint64_t limit = 4 * j + 16;
    bool *pres = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!pres) return 0;
    for (uint64_t m = 0;; m++) {
        uint64_t v = 0, p = m;
        while (p > 0) { v += p; p >>= 1; }
        if (v > limit) break;
        pres[v] = true;
    }
    uint64_t cnt = 0, res = 0;
    for (uint64_t x = 1; x <= limit; x++) {
        if (!pres[x]) {
            if (++cnt == j) { res = x; break; }
        }
    }
    free(pres);
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* find diagonal d with d(d-1)/2 < n <= d(d+1)/2 */
    uint64_t d = (uint64_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    if (d < 1) d = 1;
    while (d * (d + 1) / 2 < (uint64_t)n) d++;
    while (d > 1 && (d - 1) * d / 2 >= (uint64_t)n) d--;
    uint64_t k = (uint64_t)n - d * (d - 1) / 2;   /* 1..d */
    uint64_t row = k;
    uint64_t col = d + 1 - k;

    seq_t val;
    if (row == 1) val = (seq_t)1;
    else val = (seq_t)A055938_f(row - 1);
    for (uint64_t c = 2; c <= col; c++) {
        val = A005187_f(val + 1);
    }
    return val;
}
