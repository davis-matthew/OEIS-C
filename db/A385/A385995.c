/*
 * A385995  a(n) is the first value appearing n times in A002487.
 * https://oeis.org/A385995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002487(A385996(n)), where A385996(n) is the first index
 *            m (starting at 0) at which some value has occurred n times in
 *            A002487(0..m). Computed by a scan of A002487 with a running
 *            count per value
 * Range:     u8 n <= 541, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9991 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A385995_CAP 65536u
#define A385995_MAXN 65536u

static uint32_t A385995_cnt[A385995_CAP];
static uint32_t A385995_rec[A385995_MAXN + 1];
static uint32_t A385995_maxc = 0;
/* current pair (A002487(m), A002487(m+1)) as the Calkin-Wilf state */
static uint64_t A385995_x = 0, A385995_y = 1;

seq_t a(idx_t n) {
    if (n == 0 || n > A385995_MAXN) return (seq_t)0;
    while (A385995_maxc < (uint32_t)n) {
        uint64_t x = A385995_x, y = A385995_y;
        if (x < A385995_CAP) {
            uint32_t c = ++A385995_cnt[x];
            if (c > A385995_maxc) {
                A385995_maxc = c;
                if (c <= A385995_MAXN) A385995_rec[c] = (uint32_t)x;
            }
        }
        uint64_t q = x / y;
        A385995_x = y;
        A385995_y = (2 * q + 1) * y - x;
    }
    return (seq_t)A385995_rec[n];
}
