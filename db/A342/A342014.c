/*
 * A342014  Arithmetic derivative of n, taken modulo n: a(n) = A003415(n) mod
 *          n.
 * https://oeis.org/A342014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic_derivative(n) mod n Arithmetic derivative
 *            definition: d/dn (p^e) = e * p^(e-1) For n = ∏p_i^e_i, d/dn = n
 *            * Σ e_i / p_i
 * Range:     u8 n <= 311, u16 n <= 9738, u32 n <= 9738, u64 n <= 9738,
 *            u128 n <= 9738, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t N = (uint64_t)n;      /* absolute value of n (fits in uint64_t) */
    uint64_t res = 0;
    uint64_t remaining = N;

    /* Factor n by trial division. For n up to ~10 000 this is fast enough. */
    for (uint64_t p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            uint64_t e = 0;
            while (remaining % p == 0) {
                ++e;
                remaining /= p;
            }
            res += e * (N / p);
        }
    }

    /* If a prime factor greater than sqrt(n) remains. */
    if (remaining > 1) {
        res += N / remaining;   /* e = 1 for this factor */
    }

    uint64_t r = res % N;

    return (seq_t)r;  /* cast to the output type; harmless if seq_t is smaller */
}
