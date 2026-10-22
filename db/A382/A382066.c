/*
 * A382066  a(n) = Sum_{k=1..prime(n)-1} (-k/prime(n)) * 3^(k-1) / 2, where
 *          (p/q) is the Legendre symbol of p and q.
 * https://oeis.org/A382066
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..prime(n)-1} ( (-k | prime(n)) * 3^(k-1) ) / 2 *
 *            where (x | y) is the Legendre symbol of x modulo the odd prime
 *            y. * The implementation uses a 128‑bit accumulator for any seq_t
 *            that fits in * 64 bits, and a generic seq_t accumulator for
 *            larger builds
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"   /* int jacobi_symbol(uint64_t a, uint64_t n); */
#include "../utils/get_prime.h"       /* uint32_t get_prime(idx_t k); */

seq_t a(idx_t n)
{
    /* Undefined for n < 2 – the caller may supply any values, but the OEIS
       offset starts at 2.  Returning 0 for n < 2 keeps the function safe. */
    if (n < 2)
        return 0;

    /* Prime corresponding to index n.  get_prime returns a 32‑bit value. */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;               /* promote to seq_t (unused but kept
                                            for clarity). */

    /* If seq_t is 64 bits or smaller we use an __int128 accumulator to
       avoid overflow of intermediate powers of 3. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sum = 0;
        unsigned __int128 pow = 1;      /* 3^0 */

        for (idx_t k = 1; k < (idx_t)p32; ++k) {
            /* Legendre symbol (-k | p) = (-1 | p) * (k | p).
               Since (-1 | p) = 1 if p ≡ 1 (mod 4), -1 if p ≡ 3 (mod 4),
               we compute (-k | p) directly as the Jacobi symbol of
               (p-k) modulo p. */
            int L = jacobi_symbol((uint64_t)(p32 - k), (uint64_t)p32);

            if (L > 0)
                sum += pow;
            else if (L < 0)
                sum -= pow;

            pow *= 3;                    /* next power of 3 */
        }

        sum /= 2;                         /* final division by 2 */
        return (seq_t)sum;                 /* lower 64 bits are the answer */
    }

    /* For seq_t large enough or arbitrary‑precision, use the seq_t type
       directly.  In those builds powers of 3 can be represented exactly
       in seq_t, so no extra precision is needed. */
    else {
        seq_t sum = 0;
        seq_t pow = (seq_t)1;             /* 3^0 */

        for (idx_t k = 1; k < (idx_t)p32; ++k) {
            int L = jacobi_symbol((uint64_t)(p32 - k), (uint64_t)p32);

            if (L > 0)
                sum += pow;
            else if (L < 0)
                sum -= pow;

            pow *= (seq_t)3;              /* next power of 3 */
        }

        sum /= (seq_t)2;
        return sum;
    }
}
