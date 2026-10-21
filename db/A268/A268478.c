/*
 * A268478  L(p) modulo p^2, where p = prime(n) and L is a Lucas number
 *          (A000032).
 * https://oeis.org/A268478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L(p) mod p^2, where p = prime(n)
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 5290, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (L(p) is the p‑th Lucas number, 1‑based indexing)   */
/* The implementation uses fast doubling for Fibonacci mod    */
/* and a safe modular multiplication that works for all      */
/* seq_t types: integer or arbitrary‑precision.               */

#include "../utils/get_prime.h"

/* Multiplication modulo 'mod', safe for any seq_t size.     */
/* Uses 128‑bit intermediate if available; otherwise      */
/* falls back to an addition‑based algorithm.              */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
#if defined(__SIZEOF_INT128__) || defined(__int128_t) || defined(__uint128_t)
    return (seq_t)(((__uint128_t)a * b) % mod);
#else
    seq_t result = 0;
    while (b)
    {
        if (b & 1)
        {
            result += a;
            if (result >= mod) result -= mod;
        }
        a <<= 1;
        if (a >= mod) a -= mod;
        b >>= 1;
    }
    return result;
#endif
}

/* fast‑doubling Fibonacci: after call, *fn = F(n), *fn1 = F(n+1)  */
/* all operations are performed modulo 'mod'.                       */
static void fib_pair(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0)
    {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, mod, &a, &b);        /* a = F(k), b = F(k+1) */

    /* t = 2*F(k+1) - F(k)  (mod mod)  */
    seq_t two_b = b + b;
    if (two_b >= mod) two_b -= mod;

    seq_t t;
    if (a > two_b)
        t = two_b + mod - a;
    else
        t = two_b - a;

    seq_t c = mul_mod(a, t, mod);                      /* F(2k)   */
    seq_t d = mul_mod(a, a, mod);
    d += mul_mod(b, b, mod);
    if (d >= mod) d -= mod;                            /* F(2k+1) */

    if ((n & 1) == 0)          /* even   */
    {
        *fn  = c;
        *fn1 = d;
    }
    else                       /* odd    */
    {
        *fn  = d;
        seq_t tmp = c + d;
        if (tmp >= mod) tmp -= mod;
        *fn1 = tmp;
    }
}

/* Main sequence function: a(n) = L(p) mod p^2, with p = prime(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined, offset is 1 */

    uint32_t p   = get_prime(n);
    seq_t    mod = (seq_t)p * (seq_t)p;     /* p^2 */

    seq_t fp, fp1;                          /* F(p), F(p+1) */
    fib_pair((idx_t)p, mod, &fp, &fp1);

    /* L(p) = 2*F(p+1) - F(p)  (mod mod) */
    seq_t two_fp1 = fp1 + fp1;
    if (two_fp1 >= mod) two_fp1 -= mod;

    seq_t l;
    if (two_fp1 >= fp)
        l = two_fp1 - fp;
    else
        l = mod - (fp - two_fp1);          /* safe from overflow */

    return l;
}
