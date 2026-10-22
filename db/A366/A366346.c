/*
 * A366346  a(n) = A002070(n) + A036689(n).
 * https://oeis.org/A366346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   get_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A366346
 *  a(n) = A002070(n) + A036689(n)
 *
 *  Equivalent formula (valid for n ≥ 1):
 *      p = prime(n)
 *      a(n) = p^2 - p - Σ_{x=0}^{p-1} χ(1 + 4·(x^3 - x^2) mod p)
 *      where χ(·) is the Legendre symbol modulo the odd prime p.
 *
 *  In the special case p = 2 the value is 0.
 *
 *  The function below implements this formula and works for all
 *  supported `seq_t` and `idx_t` types.  It is compatible with
 *  C11 and C++17 and uses only the utilities listed in the
 *  problem statement.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t) */
#include "../utils/powmod64.h"   /* uint64_t powmod64(uint64_t, uint64_t, uint64_t) */

/* Legendre symbol (a|p) for odd prime p. Returns -1, 0, or 1. */
static inline int8_t legendre_modp(uint64_t a, uint64_t p)
{
    a %= p;
    if (a == 0) return 0;
    uint64_t exp = (p - 1) >> 1;
    uint64_t e = powmod64(a, exp, p);
    if (e == p - 1) return -1;
    return 1;          /* e == 1 */
}

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑indexed). */
    uint64_t p = (uint64_t)get_prime(n);

    /* Handle the only exceptional prime (p = 2). */
    if (p == 2) return (seq_t)0;

    /* Sum of Legendre symbols over all residues x. */
    int64_t sum_leg = 0;
    for (uint64_t x = 0; x < p; ++x) {
        uint64_t x2 = (x * x) % p;
        uint64_t x3 = (x2 * x) % p;
        uint64_t diff = (x3 + p - x2) % p;          /* x^3 - x^2 mod p */
        uint64_t d    = (1 + (4 * diff) % p) % p;   /* 1 + 4·(x^3 - x^2) mod p */
        sum_leg += (int64_t)legendre_modp(d, p);
    }

    /* a(p) = p^2 - p - sum_leg. */
    int64_t ans_int = (int64_t)p * (int64_t)p - (int64_t)p - sum_leg;
    return (seq_t)ans_int;
}
