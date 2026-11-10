/*
 * A333849  a(n) = gcd(A333848(n), 2*(2*n+1)), for n >= 0.
 * https://oeis.org/A333849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(A333848(n), 2*(2*n+1)), where A333848(n) = Sum_{k
 *            odd, 1<=k<2n+1, gcd(k,2n+1)=1} k; the sum is kept mod 2*(2n+1)
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9692 (2026-11-08)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t N = 2 * (seq_t)n + 1;
    seq_t M = 2 * N;
    seq_t sum = 0;
    for (seq_t k = 1; k < N; k += 2)
    {
        if (gcd(k, N) == 1)
        {
            sum += k;
            if (sum >= M)
                sum -= M;
        }
    }
    return gcd(sum, M);
}
