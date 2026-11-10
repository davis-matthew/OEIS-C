/*
 * A259428  Numbers never appearing in A259124.
 * https://oeis.org/A259428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer k that is never a value of A259124
 *            (computed by sieving A259124(m) for m up to ((V+2)/2)^2,
 *            doubling the bound V)
 * Range:     u8 n <= 35, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t V = 128;
    for (;;)
    {
        uint64_t half = (V + 2) / 2;
        uint64_t M = half * half + 2;
        uint64_t *s = (uint64_t *)calloc((size_t)(M + 1), sizeof(uint64_t));
        unsigned char *mark = (unsigned char *)calloc((size_t)(V + 1), 1);
        if (!s || !mark)
        {
            free(s);
            free(mark);
            return 0;
        }
        for (uint64_t d = 3; d * d <= M; d++)
            for (uint64_t e = d; d * e <= M; e++)
                s[d * e] += d + e - 2;
        for (uint64_t m = 3; m <= M; m++)
            if (s[m] > 0 && s[m] <= V)
                mark[s[m]] = 1;
        uint64_t cnt = 0;
        seq_t result = 0;
        int found = 0;
        for (uint64_t v = 1; v <= V; v++)
        {
            if (!mark[v])
            {
                cnt++;
                if (cnt == (uint64_t)n)
                {
                    result = (seq_t)v;
                    found = 1;
                    break;
                }
            }
        }
        free(s);
        free(mark);
        if (found)
            return result;
        V *= 2;
    }
}
