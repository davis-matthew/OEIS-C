/*
 * A379270  Numbers with only digits "1" and three digits "0".
 * https://oeis.org/A379270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007088(A379269(n)) Numbers with only digits "1" and
 *            exactly three digits "0". The first value is a(1) = 1000
 * Range:     u8 none, u16 n <= 5, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   comb2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/comb2.h"

static inline idx_t comb3(idx_t n) { return n * (n - 1) * (n - 2) / 6; }

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset starts at 1 */

    /* Find the digit length L such that the nth number lies in that block. */
    idx_t cumulative = 0;
    idx_t L = 4;                       /* minimal length: 1 followed by 3 zeros */
    while (1) {
        idx_t add = comb3(L - 1);      /* C(L-1,3) numbers of this length */
        if (cumulative + add >= n) break;
        cumulative += add;
        ++L;
    }

    /* Rank of the desired number within its block (0‑based). */
    idx_t rank   = n - cumulative - 1;
    idx_t npos   = L - 1;               /* free positions after the leading 1 */
    const idx_t K = 3;                  /* exactly three zero digits */

    idx_t zeros[K];                     /* absolute positions of the zeros */
    idx_t remaining = rank;
    idx_t start = 1;                    /* candidate index in the free area */

    for (idx_t i = 0; i < K; ++i) {
        for (idx_t cand = start; cand <= npos - (K - i) + 1; ++cand) {
            idx_t cnt;
            switch (K - i - 1) {
                case 0: cnt = 1; break;
                case 1: cnt = npos - cand; break;
                case 2: cnt = comb2(npos - cand); break;
                default: cnt = 0; break;
            }
            if (remaining < cnt) {
                zeros[i] = cand + 1;   /* convert to absolute digit position */
                start = cand + 1;
                break;
            } else {
                remaining -= cnt;
            }
        }
    }

    /* Build the decimal value from its digits. */
    seq_t value = 0;
    for (idx_t pos = 1; pos <= L; ++pos) {
        seq_t d = 1;
        if (pos == zeros[0] || pos == zeros[1] || pos == zeros[2]) d = 0;
        value = value * 10 + d;
    }
    return value;
}
