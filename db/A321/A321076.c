/*
 * A321076  One of the two successive approximations up to 11^n for 11-adic
 *          integer sqrt(5). Here the 4 (mod 11) case (except for n = 0).
 * https://oeis.org/A321076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the 11-adic integer square root of 5 congruent to 4 (mod 11).
 * It is computed by iteratively lifting the solution modulo 11^k using Hensel's lemma.
 * a(0) = 0, a(1) = 4, a(n>0) satisfies a(n)^2 ≡ 5 (mod 11^n) and  a(n) ≡ 4 (mod 11).
 */

#include <stdint.h>
#include "../utils/mul_small.h"

#ifdef __cplusplus
#include <type_traits>
#endif

/* Safe modular multiplication that avoids overflow for built‐in integer
   types of up to 64 bits by using unsigned __int128 when available. */
/* C implementation (built‑in types) */
#ifndef __cplusplus
static inline seq_t modmul(seq_t a, seq_t b, seq_t mod)
{
    unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;
    return (seq_t)(prod % (unsigned __int128)mod);
}
#else
/* C++ implementation – choose the calculation method depending on the
   nature of seq_t. */
static inline seq_t modmul(seq_t a, seq_t b, seq_t mod)
{
    if constexpr (std::is_integral<seq_t>::value && sizeof(seq_t) <= 8) {
        unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;
        return (seq_t)(prod % (unsigned __int128)mod);
    } else {
        return (a * b) % mod;
    }
}
#endif

/* --- main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* a(0) == 0 */

    /* start with the root modulo 11 (4) */
    seq_t x = 4;
    seq_t p = 11;          /* 11^1 */

    for (idx_t k = 1; k < n; ++k) {
        /* modulus for the next level: 11^(k+1) = 11 * p */
        seq_t m = mul_small(p, 11);          /* m = p * 11 */

        /* compute x^2 modulo m, safely avoiding overflow */
        seq_t x2_mod = modmul(x, x, m);

        /* fdiv = (x^2 - 5) / p  (integer division) */
        seq_t temp = x2_mod;
        if (temp < (seq_t)5)
            temp += m;                      /* ensure non‑negative before subtraction */
        seq_t fdiv = (temp - (seq_t)5) / p;   /* quotient 0…10 */

        /* fdiv modulo 11 (actually fdiv is already 0…10) */
        seq_t fdiv_mod11 = fdiv % (seq_t)11;

        /* inverse of 2*x modulo 11 is constant 7 (because x ≡ 4 (mod 11)) */
        const seq_t inv = (seq_t)7;

        /* t = -(fdiv_mod11) * inv  (mod 11) */
        seq_t t = ((seq_t)11 - fdiv_mod11) % (seq_t)11;
        t = (t * inv) % (seq_t)11;           /* t is 0…10 */

        /* lift x to the next modulus: x += t * p */
        seq_t add = mul_small(p, (unsigned int)t);
        x += add;

        /* update p to 11^(k+1) */
        p = m;
    }
    return x;
}
