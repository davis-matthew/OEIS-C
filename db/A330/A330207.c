/*
 * A330207  Chebyshev pseudoprimes to base 3: composite numbers k such that
 *          T(k, 3) == 3 (mod k), where T(k, x) is the k-th Chebyshev
 *          polynomial of the first kind.
 * https://oeis.org/A330207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth composite integer k such that T_k(3) ≡ 3 (mod
 *            k), where T_k(x) is the k‑th Chebyshev polynomial of the first
 *            kind. This implementation searches for the pseudoprimes by
 *            testing successive integers
 * Range:     u8 n <= 4, u16 n <= 141, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include utilities for primality testing and cache allocation.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_cache.h"

/*  Matrix structure for the recurrence T_n(x)=2x*T_{n-1}(x)-T_{n-2}(x).  */
typedef struct { uint64_t a00, a01, a10, a11; } Mat;

/*  Multiply two 2×2 matrices modulo mod.  */
static inline Mat mat_mul(const Mat *m1, const Mat *m2, uint64_t mod)
{
    Mat r;
    r.a00 = ( (unsigned __int128)m1->a00 * m2->a00
            + (unsigned __int128)m1->a01 * m2->a10 ) % mod;
    r.a01 = ( (unsigned __int128)m1->a00 * m2->a01
            + (unsigned __int128)m1->a01 * m2->a11 ) % mod;
    r.a10 = ( (unsigned __int128)m1->a10 * m2->a00
            + (unsigned __int128)m1->a11 * m2->a10 ) % mod;
    r.a11 = ( (unsigned __int128)m1->a10 * m2->a01
            + (unsigned __int128)m1->a11 * m2->a11 ) % mod;
    return r;
}

/*  Fast exponentiation of a matrix modulo mod.  */
static inline Mat mat_pow(Mat base, uint64_t exp, uint64_t mod)
{
    Mat result = {1, 0, 0, 1};               /* identity matrix */
    while (exp) {
        if (exp & 1) result = mat_mul(&result, &base, mod);
        base = mat_mul(&base, &base, mod);
        exp >>= 1;
    }
    return result;
}

/*  Compute T_n(3) modulo n using matrix exponentiation.  */
static seq_t T_mod(uint64_t n)
{
    if (n <= 1) return (seq_t)n;             /* not used for composites */

    uint64_t mod = n;
    uint64_t x   = 3 % mod;                   /* x = T_1(3) mod n */

    uint64_t m00 = (2 * x) % mod;             /* 2x  mod n */
    uint64_t m01 = (mod - 1) % mod;           /* -1 mod n */

    Mat M = {m00, m01, 1 % mod, 0};
    Mat P = mat_pow(M, n - 1, mod);           /* M^(n-1) */

    uint64_t Tk = ( (unsigned __int128)P.a00 * x
                  + (unsigned __int128)P.a01 * 1 ) % mod;
    return (seq_t)Tk;
}

/*  Static storage for discovered pseudoprimes.  */
static seq_t *cache = NULL;        /* array of pseudoprime values */
static size_t  cache_cap = 0;     /* allocated capacity of cache   */
static size_t  cache_len = 0;     /* number stored in cache        */
static uint64_t current   = 2;    /* next integer to test          */

/*  Guarantee that cache has room for at least n elements.  */
static void ensure_cache_capacity(size_t n)
{
    if (cache_cap < n)
        ensure_cache((idx_t)n, &cache, &cache_cap);
}

/*  Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n <= 0 */

    /*  Generate pseudoprimes until we have at least n terms.  */
    while (cache_len < (size_t)n) {
        if (!is_prime((idx_t)current)) {           /* skip primes      */
            seq_t tmod   = T_mod(current);
            seq_t target = (seq_t)(3 % current);
            if (tmod == target) {                   /* pseudoprime found */
                ensure_cache_capacity(cache_len + 1);
                cache[cache_len++] = current;
            }
        }
        current++; /* move to next integer */
    }
    return cache[n - 1];   /* 1‑based indexing */
}
