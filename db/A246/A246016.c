/*
 * A246016  a(n) = (-1)^A055941(n).
 * https://oeis.org/A246016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^A055941(n), where A055941(n) = Sum_{j} (i(j) - j)
 *            for n = Sum_{j} 2^i(j)
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 85 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A055941_a(idx_t n)
{
    idx_t result = 0;
    idx_t zero_cnt = 0;
    idx_t temp = n;
    while (temp)
    {
        if (temp & 1ull)
            result += zero_cnt;
        else
            zero_cnt++;
        temp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    return (A055941_a(n) & 1ull) ? (seq_t)-1 : (seq_t)1;
}
