/*
 * A280131  Partial sums of A029940 (Product_{d|n} phi(d)).
 * https://oeis.org/A280131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} ( product_{d|i} φ(d) ) (φ = Euler totient)
 * Range:     u8 n <= 15, u16 n <= 43, u32 n <= 119, u64 n <= 359,
 *            u128 n <= 1259, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* The following function has external linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                     /* offset is 1; safety for n==0 */

    /* Allocate array for Euler totients up to n (uint64_t is suitable for 0..n). */
    uint64_t *phi = (uint64_t*)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!phi) return (seq_t)0;                       /* out‑of‑memory guard */

    /* Initialise φ[i] = i.  φ[0] and φ[1] are handled correctly. */
    for (idx_t i = 0; i <= n; ++i) phi[i] = i;

    /* Compute φ[1..n] by a linear (Euler) sieve.  Complexity O(n log log n). */
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == p) {                           /* p is prime */
            for (idx_t m = p; m <= n; m += p) {
                phi[m] -= phi[m] / p;
            }
        }
    }

    /* Now evaluate the sum a(n). */
    seq_t sum = 0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t prod = 1;

        /* Enumerate divisors d of i up to √i.  No floating point or math.h needed. */
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                prod *= (seq_t)phi[d];              /* φ(d) */
                idx_t other = i / d;
                if (other != d) {
                    prod *= (seq_t)phi[other];      /* φ(i/d) */
                }
            }
        }

        sum += prod;
    }

    free(phi);
    return sum;
}
