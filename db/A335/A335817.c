/*
 * A335817  a(k) is the index of the first occurrence of 2*k-1 in A208884, or
 *          0 if it does not occur.
 * https://oeis.org/A335817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(k) = least n >= 1 with A208884(n) = 2*k-1, where A208884(1) =
 *            1 and A208884(n) = odd_part(A208884(n-1) + n); computed by
 *            scanning A208884 and recording first occurrences (0 never
 *            returned; the search runs until found)
 * Range:     u8 none, u16 none, u32 n <= 3762, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9301 (2026-11-08)
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/odd_part.h"

#define A335817_TS (1 << 17)

seq_t a(idx_t k)
{
    static idx_t tab[A335817_TS];
    static idx_t pos = 0;
    static seq_t cur = 1;

    if (k == 0) return 0;

    if (k < (idx_t)A335817_TS) {
        while (tab[k] == 0) {
            pos++;
            if (pos == 1) cur = (seq_t)1;
            else cur = odd_part(cur + (seq_t)pos);
            if (cur < (seq_t)(2 * A335817_TS)) {
                long long kk = (long long)((cur + 1) / 2);
                if (kk >= 1 && kk < A335817_TS && tab[kk] == 0) tab[kk] = pos;
            }
        }
        return (seq_t)tab[k];
    } else {
        seq_t target = (seq_t)k * 2 - 1;
        seq_t c = 1;
        idx_t p = 1;
        while (c != target) {
            p++;
            c = odd_part(c + (seq_t)p);
        }
        return (seq_t)p;
    }
}
