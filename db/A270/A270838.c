/*
 * A270838  Largest value k < p-2 such that k! mod p = 1 with p = prime(n).
 * https://oeis.org/A270838
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k < p-2 such that k! mod p = 1, with p = prime(n)
 * Range:     u8 n <= 63, u16 n <= 4555, u32 n <= 4555, u64 n <= 4555,
 *            u128 n <= 4555, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* Modular multiplication using 128‑bit intermediate for 64‑bit arguments */
static inline uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

seq_t a(idx_t n)
{
    if (n < 3U)
        return 0;                    /* undefined below the offset */

    /* p may safely fit into 32 bits for the range we consider; use 64‑bit for safety */
    uint64_t p = (uint64_t)get_prime(n);

    if (p < 5U)                      /* for n ≥ 3 we always have p ≥ 5 */
        return 0;

    uint64_t fact   = 1;
    uint64_t best_k = 1;             /* 1! = 1 is always a candidate */

    /* Search k = 1 .. p-3 (i.e., k < p-2) */
    for (uint64_t k = 1ULL; k < p - 2ULL; ++k)
    {
        fact = mod_mul_uint64(fact, k, p);

        if (fact == 1ULL)
            best_k = k;
    }

    return (seq_t)best_k;
}
