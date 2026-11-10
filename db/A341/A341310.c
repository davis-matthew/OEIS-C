/*
 * A341310  Sum of odd divisors of n that are > A003056(n).
 * https://oeis.org/A341310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d odd, d > A003056(n)} d, where A003056(n) =
 *            floor((sqrt(8n+1)-1)/2); computed by trial division up to
 *            sqrt(n)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A341310_tri_inv(uint64_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 33;
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= (unsigned __int128)n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t k = A341310_tri_inv(N);
    seq_t s = 0;
    for (uint64_t i = 1; i * i <= N; i++) {
        if (N % i == 0) {
            uint64_t j = N / i;
            if ((i & 1) && i > k)
                s += (seq_t)i;
            if (j != i && (j & 1) && j > k)
                s += (seq_t)j;
        }
    }
    return s;
}
