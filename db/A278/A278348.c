/*
 * A278348  Number of 2 X 2 singular integer matrices with elements from
 *          {0,...,n} with no elements repeated.
 * https://oeis.org/A278348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=2..n} 2*φ(m) * ( floor(n/m)*(floor(n/m)-1) –
 *            2*floor( floor(n/m)/m ) ) where φ is Euler's totient function
 * Range:     u8 n <= 15, u16 n <= 137, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Compute φ(i) for i = 1..n by the standard sieve. */
    size_t N = (size_t)n + 1;
    uint32_t *phi = (uint32_t *)malloc(N * sizeof(uint32_t));
    if (!phi)
        return 0;                    /* fallback in unlikely case of OOM */

    for (size_t i = 0; i < N; ++i)
        phi[i] = (uint32_t)i;
    phi[0] = 0;
    phi[1] = 1;

    for (size_t i = 2; i < N; ++i) {
        if (phi[i] == i) {          /* i is prime */
            for (size_t j = i; j < N; j += i)
                phi[j] -= phi[j] / i;
        }
    }

    seq_t result = 0;
    for (idx_t m = 2; m <= n; ++m) {
        seq_t S = n / m;            /* floor(n/m) */
        if (S < 2)
            continue;               /* no valid k,l pairs */

        seq_t sub = (S / m) * 2;    /* 2*floor(S/m) */
        seq_t T   = S * (S - 1) - sub;
        result += (seq_t)2 * (seq_t)phi[m] * T;
    }

    free(phi);
    return result;
}
