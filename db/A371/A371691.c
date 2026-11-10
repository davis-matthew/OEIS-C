/*
 * A371691  Main diagonal of A365992: parity of the n-th term in the
 *          trajectory of n under the A185452 map.
 * https://oeis.org/A371691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A368301(n) mod 2 = parity of T^(n-1)(n), where * T(x) =
 *            x/2 if x even, (5x+1)/2 if x odd (the A185452 map). * The first
 *            k parities of a trajectory depend only on x mod 2^k, so the map
 *            * is run on x mod 2^(64W) in fixed-width words (W*64 >= n+1).
 *            Each step * loses one correct high bit. Afte...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 8085 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t W = (size_t)(n / 64) + 2;
    uint64_t *w = (uint64_t *)malloc(W * sizeof(uint64_t));
    size_t i;
    idx_t step;
    seq_t r;

    for (i = 0; i < W; ++i)
        w[i] = 0;
    w[0] = (uint64_t)n;

    for (step = 1; step < n; ++step)
    {
        if (w[0] & 1)
        {
            /* x = 5x + 1 (mod 2^(64W)) */
            uint64_t carry = 1;
            for (i = 0; i < W; ++i)
            {
                uint64_t x = w[i];
                uint64_t hi = x >> 62;
                uint64_t lo = x << 2;
                uint64_t s = lo + x;
                uint64_t c1 = (s < lo) ? 1 : 0;
                uint64_t s2 = s + carry;
                uint64_t c2 = (s2 < s) ? 1 : 0;
                w[i] = s2;
                carry = hi + c1 + c2;
            }
        }
        /* x = x / 2 */
        for (i = 0; i + 1 < W; ++i)
            w[i] = (w[i] >> 1) | (w[i + 1] << 63);
        w[W - 1] >>= 1;
    }

    r = (seq_t)(w[0] & 1);
    free(w);
    return r;
}
