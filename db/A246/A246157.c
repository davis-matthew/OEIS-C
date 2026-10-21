/*
 * A246157  Reducible polynomials over GF(2) which are both odd and odious
 *          when coded in binary, or equally, which have an odd number of
 *          nonzero terms, with the constant term being 1.
 * https://oeis.org/A246157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sequence A246157 (numbers whose binary polynomial * is
 *            reducible over GF(2), is odd and odious). This implementation *
 *            computes terms on demand by scanning integers and testing the *
 *            three conditions using polynomial arithmetic over GF(2)
 * Range:     u8 n <= 24, u16 n <= 9726, u32 n <= 9726, u64 n <= 9726,
 *            u128 n <= 9726, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Polynomial arithmetic over GF(2) for 64‑bit monic polynomials. */

/* Highest set bit position.  Assumes x > 0. */
static inline unsigned int deg_uint64(uint64_t x)
{
    return 63 - __builtin_clzll(x);
}

/* Multiply a and b modulo the monic polynomial 'mod'. */
static uint64_t mulmod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    uint64_t res = 0;
    unsigned int deg_mod = deg_uint64(mod);
    uint64_t mask = 1ULL << deg_mod;          /* degree‑overflow bit */

    while (a) {
        if (a & 1ULL)        /* add current b */
            res ^= b;
        a >>= 1;
        b <<= 1;
        if (b & mask)        /* reduce if needed */
            b ^= mod;
    }
    return res;
}

/* Remainder of a divided by b.  Both are non‑zero monic polynomials. */
static uint64_t poly_mod(uint64_t a, uint64_t b)
{
    unsigned int deg_b = deg_uint64(b);
    unsigned int deg_a, shift;
    while (a && (deg_a = deg_uint64(a)) >= deg_b) {
        shift = deg_a - deg_b;
        a ^= b << shift;
    }
    return a;
}

/* GCD of two polynomials over GF(2). */
static uint64_t poly_gcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* Compute x^(2^k) mod f, where x is the polynomial 'x' (i.e. mask 2). */
static uint64_t pow_x_pow(uint64_t f, unsigned int k)
{
    uint64_t result = 2ULL; /* polynomial x */
    for (unsigned int i = 0; i < k; ++i)
        result = mulmod_u64(result, result, f);
    return result;
}

/* Test whether the monic polynomial represented by 'f' is irreducible
 * over GF(2).  f must be > 1 and monic. */
static bool is_irreducible(uint64_t f)
{
    unsigned int n = deg_uint64(f);

    /* Necessary condition: f divides x^(2^n) - x */
    uint64_t xp = pow_x_pow(f, n);
    if ((xp ^ 2ULL) != 0ULL)          /* not equal to x */
        return false;                /* reducible */

    /* For each prime divisor p of n, check gcd(x^(2^(n/p)) - x, f) == 1 */
    unsigned int tmp = n;
    for (unsigned int p = 2; (uint64_t)p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            unsigned int exp = n / p;
            uint64_t to_test = pow_x_pow(f, exp);
            if (poly_gcd(to_test ^ 2ULL, f) != 1ULL)
                return false;        /* reducible */
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {                    /* remaining prime factor */
        unsigned int exp = n / tmp;
        uint64_t to_test = pow_x_pow(f, exp);
        if (poly_gcd(to_test ^ 2ULL, f) != 1ULL)
            return false;
    }
    return true;                       /* irreducible */
}

/* Main routine: returns the n-th term of A246157 (1‑based index). */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static uint64_t cur = 1;          /* next integer to test */

    /* Lazy allocation of the result array. */
    if (seq == NULL) {
        cap = 16;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    while (len < n) {
        uint64_t x = cur++;
        /* Must be odd. */
        if ((x & 1ULL) == 0) continue;
        /* Must be odious (odd number of 1‑bits). */
        if ((__builtin_popcountll(x) & 1ULL) == 0) continue;
        /* Must be reducible over GF(2). */
        if (is_irreducible(x)) continue;          /* skip irreducible */

        /* Store the qualifying number. */
        if (len == cap) {
            cap <<= 1;
            seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
        }
        seq[len++] = (seq_t)x;
    }
    return seq[n - 1];
}
