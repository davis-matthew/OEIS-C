/*
 * A348473  a(n) = Sum_{k=1..A003056(n)} 2^(T(n,k)-1), where T(n,k) = k-th
 *          term in row n of A235791.
 * https://oeis.org/A348473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..A003056(n)} 2^(T(n,k)-1), where T(n,k) =
 *            ceil((n+1)/k - (k+1)/2) is the k-th term of row n of A235791
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 3169 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t kmax = 0;
    while ((kmax + 1) * (kmax + 2) / 2 <= n) kmax++;

    seq_t result = 0;
    seq_t p = 1;          /* 2^e */
    idx_t e = 0;
    /* t(k) is nonincreasing in k, so go from kmax down to 1 for increasing t */
    for (idx_t k = kmax; k >= 1; k--) {
        idx_t num = 2 * (n + 1) - k * (k + 1);
        idx_t den = 2 * k;
        idx_t t = (num + den - 1) / den;
        while (e < t - 1) {
            p = p * 2;
            e++;
        }
        result = result + p;
    }
    return result;
}
