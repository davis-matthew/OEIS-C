/*
 * A246156  Odd reducible polynomials over GF(2), coded in binary.
 *          (Polynomials with the constant term 1 that are reducible over
 *          GF(2)).
 * https://oeis.org/A246156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 7914, u32 n <= 7914, u64 n <= 7914,
 *            u128 n <= 7914, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A246156: a(n) is the n‑th odd integer whose binary
   representation encodes a reducible polynomial over GF(2).  The
   function generates terms by iteratively testing odd candidates for
   reducibility using the Rabin irreducibility test. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* bit length of an integer */
static size_t bitlen_seq_t(seq_t x)
{
    size_t len = 0;
    while (x) { x >>= 1; ++len; }
    return len;
}

/* remainder of a divided by b (both over GF(2)) */
static seq_t poly_mod(seq_t a, seq_t b)
{
    size_t db = bitlen_seq_t(b);
    if (b == 0) return a;
    while (a)
    {
        size_t da = bitlen_seq_t(a);
        if (da < db) break;
        a ^= b << (da - db);
    }
    return a;
}

/* gcd of two polynomials over GF(2) */
static seq_t poly_gcd(seq_t a, seq_t b)
{
    while (b)
    {
        seq_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* multiplication of a and b modulo mod (all over GF(2)) */
static seq_t poly_mul_mod(seq_t a, seq_t b, seq_t mod)
{
    seq_t res = 0;
    while (b)
    {
        if (b & 1) res ^= a;
        b >>= 1;
        a <<= 1;
        a = poly_mod(a, mod);
    }
    return poly_mod(res, mod);
}

/* x^(2^k) modulo f, where x is represented by the binary integer 2 */
static seq_t poly_pow_x2k(seq_t f, idx_t k)
{
    seq_t g = 2ULL;                   /* x */
    for (idx_t i = 0; i < k; ++i)
        g = poly_mul_mod(g, g, f);
    return g;
}

/* test whether f is irreducible over GF(2) */
static bool is_irreducible(seq_t f)
{
    if (f == 0) return false;
    size_t n = bitlen_seq_t(f) - 1;          /* degree of f */
    if (n <= 1) return true;                 /* linear polynomials are irreducible */

    /* factor n into its prime divisors */
    idx_t tmp = (idx_t)n;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            size_t m = n / p;
            seq_t g = poly_pow_x2k(f, m);
            seq_t h = g ^ 2ULL;  /* x^(2^m) - x */
            if (poly_gcd(h, f) != 1) return false;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1)
    {
        size_t m = n / tmp;
        seq_t g = poly_pow_x2k(f, m);
        seq_t h = g ^ 2ULL;
        if (poly_gcd(h, f) != 1) return false;
    }

    /* final check: x^(2^n) mod f should equal x */
    seq_t g = 2ULL;
    for (idx_t i = 0; i < n; ++i) g = poly_mul_mod(g, g, f);
    return g == 2ULL;
}

/* test whether a given odd integer encodes a reducible polynomial over GF(2) */
static bool is_reducible(seq_t x)
{
    return ((x & 1ULL) != 0) && !is_irreducible(x);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored terms */
    static idx_t cache_cap = 0;   /* current capacity */
    static idx_t cache_len = 0;   /* number of cached terms */
    static seq_t next_candidate = 5; /* next odd integer to test */

    if (n == 0) return 0; /* undefined for n<1, but we give 0 */

    /* extend cache until we have at least n terms */
    while (cache_len < n)
    {
        seq_t x = next_candidate;
        /* find the next odd reducible integer */
        while (true)
        {
            if (is_reducible(x))
            {
                /* ensure capacity */
                if (cache_len == cache_cap)
                {
                    cache_cap = cache_cap ? cache_cap << 1 : 16;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = x;
                next_candidate = x + 2;
                break;
            }
            x += 2;   /* skip even numbers */
        }
    }

    return cache[n - 1];
}
