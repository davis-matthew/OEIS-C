/*
 * A266697  Multiplicative order of 2^n mod 2*n+1.
 * https://oeis.org/A266697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002326(n)/gcd(n, A002326(n)) where A002326(n) is the
 *            multiplicative order of 2 modulo (2n+1)
 * Range:     u8 n <= 194, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

/* Compute the multiplicative order of 2 modulo an odd integer m.
 * For m==1 it returns 1, matching the sequence value a(0).           */
static seq_t multiplicative_order_mod(uint64_t m)
{
    if (m <= 1)
        return (seq_t)1;

    /* ---------- compute φ(m) ---------- */
    uint64_t phi = m;
    uint64_t tmp = m;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            phi = phi / p * (p - 1);
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)
        phi = phi / tmp * (tmp - 1);

    /* ---------- find the minimal k with 2^k ≡ 1 (mod m) ---------- */
    uint64_t order = phi;
    uint64_t factor = order;

    for (uint64_t p = 2; p * p <= factor; ++p) {
        if (factor % p == 0) {
            while ((order % p == 0) &&
                   mod_pow((seq_t)2, (uint64_t)(order / p), (seq_t)m) == (seq_t)1)
                order /= p;
            while (factor % p == 0)
                factor /= p;
        }
    }
    if (factor > 1) {                /* remaining prime factor */
        uint64_t p = factor;
        while ((order % p == 0) &&
               mod_pow((seq_t)2, (uint64_t)(order / p), (seq_t)m) == (seq_t)1)
            order /= p;
    }

    return (seq_t)order;
}

/* External function required by the framework.           */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                /* a(0) = 1 as defined */

    uint64_t m   = 2 * n + 1;            /* modulus for the order   */
    seq_t   ord  = multiplicative_order_mod(m);
    seq_t   g    = gcd((seq_t)n, ord);   /* gcd of n and the order */
    return ord / g;                     /* a(n) = ord/g            */
}
