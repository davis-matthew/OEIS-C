/*
 * A343408  Sum of proper divisors of n that are triangular numbers.
 * https://oeis.org/A343408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of proper divisors of n that are triangular numbers
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   factor_u64, is_triangular_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses integer‑sqrt based on 64‑bit arithmetic. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/is_triangular_idx.h"

seq_t a(idx_t n)
{
    /* Handle n=1 explicitly */
    if (n == 1)
        return 0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);

    /* Max number of divisors for n <= 10000 is < 200; use a fixed buffer. */
    idx_t divs[512];
    size_t dcnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        unsigned e = (unsigned)exps[i];
        size_t curcnt = dcnt;
        idx_t pow = 1;
        for (unsigned exp = 1; exp <= e; ++exp) {
            pow *= p;
            for (size_t j = 0; j < curcnt; ++j) {
                divs[dcnt++] = divs[j] * pow;
            }
        }
    }

    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divs[i];
        if (d >= n)   /* skip n itself and any d > n (should not happen) */
            continue;
        if (is_triangular_idx(d))
            sum += (seq_t)d;   /* wrap-around defined by seq_t type */
    }
    return sum;
}
