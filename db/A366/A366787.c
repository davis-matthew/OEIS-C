/*
 * A366787  a(n) = A366789(A163511(n)).
 * https://oeis.org/A366787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A366789(A163511(n)); A366789 is completely multiplicative
 *            with a(prime(k)) = oddpart(k). A163511(n) = prime(r) *
 *            Product_{i=1..r} prime(i)^(g_i), where r = popcount(n) and g_i
 *            is the gap before the i-th set bit. So a(n) = oddpart(r) *
 *            Product_i oddpart(i)^(g_i)
 * Range:     u8 n <= 130, u16 n <= 1038, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 116 terms through n = 9904 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned long long A366787_oddpart(unsigned long long k)
{
    while (k && !(k & 1ULL)) k >>= 1;
    return k;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    unsigned long long r = 0;
    idx_t tmp = n;
    idx_t prev = 0, bitpos = 1;
    seq_t result = (seq_t)1;
    unsigned long long i = 0;
    while (tmp) {
        if (tmp & 1ULL) {
            i++;
            idx_t exp = bitpos - prev - 1;
            seq_t base = (seq_t)A366787_oddpart(i);
            for (idx_t e = 0; e < exp; ++e)
                result = result * base;
            prev = bitpos;
        }
        ++bitpos;
        tmp >>= 1;
    }
    r = i;
    result = result * (seq_t)A366787_oddpart(r);
    return result;
}
