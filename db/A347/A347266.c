/*
 * A347266  a(n) is the number whose binary representation is the
 *          concatenation of terms in the n-th row of A237048.
 * https://oeis.org/A347266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Sum_{c=1..k} T(n,c)*2^(k-c), k = A003056(n) =
 *            floor((sqrt(8n+1)-1)/2), where T(n,c) = 1 if n >= c(c+1)/2 and c
 *            divides n - c(c+1)/2, else 0 (row n of A237048 read as binary
 *            digits)
 * Range:     u8 n <= 44, u16 n <= 152, u32 n <= 560, u64 n <= 2144,
 *            u128 n <= 8384, big any n
 * Validated: standard, pass, 82 terms through n = 8823 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t k = 0;
    while ((k + 1) * (k + 2) / 2 <= N)
        k++;

    seq_t v = 0;
    for (uint64_t c = 1; c <= k; c++) {
        uint64_t start = c * (c + 1) / 2;
        uint64_t bit = 0;
        if (N >= start && (N - start) % c == 0)
            bit = 1;
        v = v * 2;
        if (bit)
            v = v + 1;
    }
    return v;
}
