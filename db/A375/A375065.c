/*
 * A375065  Starting index of the first length-n (exactly) run of zeros in
 *          A374966.
 * https://oeis.org/A375065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search. Generate A374966 (a(0)=0; a(k)=0 if k appeared before,
 *            else a(k-1)+k) and record the start index of the first run of
 *            exactly n zeros. Indices start at 0, so a(0)=0 begins a run
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A375065_MAXN 64

static uint64_t A375065_first[A375065_MAXN];
static bool A375065_have[A375065_MAXN];

seq_t a(idx_t n)
{
    if (n >= A375065_MAXN)
        return (seq_t)0;
    if (A375065_have[n])
        return (seq_t)A375065_first[n];

    uint64_t cap = (uint64_t)1 << 20;
    for (;;) {
        for (int i = 0; i < A375065_MAXN; i++)
            A375065_have[i] = false;
        uint64_t *bits = (uint64_t *)calloc((size_t)(cap / 64 + 1), sizeof(uint64_t));
        if (!bits)
            abort();
        bits[0] |= 1;                       /* 0 appears as term a(0) */
        /* a(0) = 0 starts a run of zeros at index 0 */
        uint64_t prev = 0, run = 1, start = 0;
        uint64_t k;
        for (k = 1; k < cap; k++) {
            uint64_t cur;
            if ((bits[k >> 6] >> (k & 63)) & 1)
                cur = 0;
            else
                cur = prev + k;
            if (cur == 0) {
                if (run == 0) start = k;
                run++;
            } else {
                if (cur < cap)
                    bits[cur >> 6] |= (uint64_t)1 << (cur & 63);
                if (run > 0 && run < A375065_MAXN && !A375065_have[run]) {
                    A375065_have[run] = true;
                    A375065_first[run] = start;
                }
                run = 0;
            }
            prev = cur;
            if (A375065_have[n])
                break;
        }
        free(bits);
        if (A375065_have[n])
            return (seq_t)A375065_first[n];
        cap <<= 1;
    }
}
