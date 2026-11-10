/*
 * A395253  Differences of A395251.
 * https://oeis.org/A395253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A395251(n+1) - A395251(n), where A395251 lists the k >= 1
 *            with b(b(k)+b(k-1)) != k, b(k) = floor(k/sqrt(2) +
 *            2*(sqrt(2)-1)); computed by search with exact integer b(k) =
 *            isqrt(floor((k+4)^2/2)) - 2
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static uint64_t A395253_isqrt(uint64_t x)
{
    uint64_t r = (uint64_t)sqrt((double)x);
    while (r * r > x) r--;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

static uint64_t A395253_b(uint64_t k)
{
    uint64_t m = k + 4;
    return A395253_isqrt((m * m) / 2) - 2;
}

seq_t a(idx_t n)
{
    static uint64_t *P = NULL;
    static uint64_t size = 0, cap = 0, next_k = 1;

    if (n == 0) return (seq_t)0;
    while (size < (uint64_t)n + 1) {
        uint64_t t = A395253_b(A395253_b(next_k) + A395253_b(next_k - 1));
        if (t != next_k) {
            if (size == cap) {
                uint64_t nc = cap ? cap * 2 : 128;
                uint64_t *np = (uint64_t *)realloc(P, nc * sizeof(uint64_t));
                if (!np) return (seq_t)0;
                P = np;
                cap = nc;
            }
            P[size++] = next_k;
        }
        ++next_k;
    }
    return (seq_t)(P[n] - P[n - 1]);
}
