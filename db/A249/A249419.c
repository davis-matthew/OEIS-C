/*
 * A249419  Least prime p>=prime(n) such that p + 2 == 0 (mod prime(n)).
 * https://oeis.org/A249419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least prime p ≥ prime(n) such that (p + 2) ≡ 0 (mod
 *            prime(n))
 * Range:     u8 n <= 9, u16 n <= 285, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1, n=0 unused */

    /* nth prime – safe to cast to 64‑bit, nth prime up to 10 000 is < 2^32 */
    uint64_t q = (uint64_t)get_prime(n);

    /* Search for the smallest prime p = (k+1)*q – 2, with k ≥ 1 */
    for (uint64_t k = 1; ; ++k)
    {
        /* Use 128‑bit arithmetic for the product to avoid overflow,
           then cast back – the final value fits in 64 bits for n ≤ 10 000. */
        unsigned __int128 prod = (unsigned __int128)(k + 1) * q;
        uint64_t p = (uint64_t)(prod - 2);

        if (p >= q && is_prime(p))
            return (seq_t)p;
    }
}
