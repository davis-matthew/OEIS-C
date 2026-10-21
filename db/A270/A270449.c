/*
 * A270449  Odd integers n such that the sum of the Pell numbers A000129(0) +
 *          ... + A000129(n-1) is divisible by n*(n+1)/2.
 * https://oeis.org/A270449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   mod_add128, mod_sub128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A270449
   Odd integers n > 1 such that the sum of the Pell numbers
   A000129(0)+…+A000129(n-1) is divisible by n*(n+1)/2.
   The program searches odd n starting at 13 and caches the results.
   It uses the Pell sum formula  S(n) = (P_n + P_{n-1} – 1)/2. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_add128.h"
#include "../utils/mod_sub128.h"

static inline unsigned __int128
mod_mul128(unsigned __int128 a, unsigned __int128 b,
           unsigned __int128 mod)
{
    /* binary multiply modulo mod */
    unsigned __int128 res = 0;
    while (b)
    {
        if (b & 1U) res = mod_add128(res, a, mod);
        a = mod_add128(a, a, mod);
        b >>= 1;
    }
    return res;
}

/* -------------- Pell pair modulo mod -------------------------------- */
static inline void
pell_mod_pair_uint128(idx_t n, unsigned __int128 mod,
                      unsigned __int128 *pn, unsigned __int128 *pn_minus1)
{
    /* The matrix [[2,1],[1,0]]^n = [[P_{n+1}, P_n],
                                  [P_n,     P_{n-1}]] (mod mod) */
    unsigned __int128 a00 = 1, a01 = 0, a10 = 0, a11 = 1;  /* identity */
    unsigned __int128 b00 = (2 % mod), b01 = (1 % mod),
                     b10 = (1 % mod), b11 = (0 % mod);     /* base */

    idx_t e = n;
    while (e)
    {
        if (e & 1U)
        {
            unsigned __int128 na00 = mod_add128(
                    mod_mul128(a00, b00, mod),
                    mod_mul128(a01, b10, mod), mod);
            unsigned __int128 na01 = mod_add128(
                    mod_mul128(a00, b01, mod),
                    mod_mul128(a01, b11, mod), mod);
            unsigned __int128 na10 = mod_add128(
                    mod_mul128(a10, b00, mod),
                    mod_mul128(a11, b10, mod), mod);
            unsigned __int128 na11 = mod_add128(
                    mod_mul128(a10, b01, mod),
                    mod_mul128(a11, b11, mod), mod);
            a00 = na00; a01 = na01; a10 = na10; a11 = na11;
        }

        /* square the base matrix */
        unsigned __int128 nb00 = mod_add128(
                mod_mul128(b00, b00, mod),
                mod_mul128(b01, b10, mod), mod);
        unsigned __int128 nb01 = mod_add128(
                mod_mul128(b00, b01, mod),
                mod_mul128(b01, b11, mod), mod);
        unsigned __int128 nb10 = mod_add128(
                mod_mul128(b10, b00, mod),
                mod_mul128(b11, b10, mod), mod);
        unsigned __int128 nb11 = mod_add128(
                mod_mul128(b10, b01, mod),
                mod_mul128(b11, b11, mod), mod);
        b00 = nb00; b01 = nb01; b10 = nb10; b11 = nb11;

        e >>= 1;
    }

    *pn        = a01;   /* P_n   */
    *pn_minus1 = a11;   /* P_{n-1} */
}

/* ------------------- main sequence function --------------------------- */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *terms = NULL;   /* array of found terms     */
    static size_t len   = 0;      /* number of terms stored  */
    static size_t cap   = 0;      /* allocated capacity      */
    static idx_t  last = 13;      /* first odd candidate to test */

    while (len < n)
    {
        if (len > 0) last += 2;   /* step to next odd integer */

        unsigned __int128 mod = (unsigned __int128)last *
                                ((unsigned __int128)last + 1U);

        unsigned __int128 pn, pn_m1;
        pell_mod_pair_uint128(last, mod, &pn, &pn_m1);

        unsigned __int128 sum = mod_add128(pn, pn_m1, mod);
        unsigned __int128 diff = (sum >= 1U) ? sum - 1U : sum + mod - 1U;

        if (diff == 0U)                 /* condition satisfied */
        {
            if (len == cap)
            {
                cap = (cap) ? cap << 1 : 16;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[len++] = (seq_t)last;
        }
    }
    return terms[n - 1];                /* 1‑based index */
}
