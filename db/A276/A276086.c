/*
 * A276086  Primorial base exp-function: digits in primorial base
 *          representation of n become the exponents of successive prime
 *          factors whose product a(n) is.
 * https://oeis.org/A276086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = ∏_{k≥0} prime(k+1)^{d_k(n)}, where d_k(n) are the digits
 *            of n written in primorial base. a(0) = 1
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 416, u64 n <= 2308,
 *            u128 n <= 2310, big any n
 * Helpers:   big_pow, get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                       /* a(0) = 1 */

    /* a small fixed buffer is sufficient because n is small;
       primorials grow very fast. */
    const int MAX_K = 64;
    uint64_t prim[MAX_K];
    prim[0] = 1;                         /* 0‑th primorial = 1 */
    uint64_t max_k = 0;

    /* Build primorials p_k = product of first k+1 primes
       until the next would exceed the index n. */
    for (uint64_t k = 1; k < MAX_K; ++k) {
        uint64_t p = get_prime(k);        /* prime(k+1)   */
        if (prim[k-1] > UINT64_MAX / p)   /* prevent 64‑bit overflow */
            break;
        prim[k] = prim[k-1] * p;
        if (prim[k] > n)
            break;                       /* this primorial too large */
        max_k = k;                       /* record the largest valid k */
    }

    seq_t result = 1;
    uint64_t rem   = n;

    /* Iterate from most significant digit down to the least. */
    for (int k = (int)max_k; k >= 0; --k) {
        uint64_t digit = rem / prim[k];
        rem -= digit * prim[k];

        if (digit == 0)
            continue;                    /* multiplying by 1 is useless */

        seq_t base = (seq_t)get_prime(k + 1); /* prime(k+1)  */
        seq_t pw   = big_pow(base, (seq_t)digit);
        result = seq_mul(result, pw);
    }

    return result;
}
