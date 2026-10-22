/*
 * A309012  Number of ordered pairs (i,j) with 0 < i < j < prime(n)/2 such
 *          that (i^16 mod prime(n)) > (j^16 mod prime(n)).
 * https://oeis.org/A309012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (i,j) : 0 < i < j < prime(n)/2 and (i^16 mod prime(n))
 *            > (j^16 mod prime(n)) }. Uses a Fenwick tree to count inversions
 *            of the sequence v_i = i^16 (mod p)
 * Range:     u8 n <= 21, u16 n <= 170, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utilities that will be available in the build */
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt) */
#include "../utils/mul_mod64.h"           /* uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod) */

#define PRIME_LIMIT 200000      /* enough for prime(10000)=104729 */

/* Fenwick tree (prefix sum) for integer counts, size indexed up to PRIME_LIMIT+1. */
static uint32_t ft[PRIME_LIMIT + 2];
static size_t ft_size = 0;   /* number of indices actually used (p+1) */

/* Prime list: built lazily on first call. */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static bool primes_ready = false;

/* Helper: clear first 'n' entries of the Fenwick tree */
static inline void ft_clear(size_t n)
{
    for (size_t i = 1; i <= n; ++i)
        ft[i] = 0;
}

/* Helper: add delta to position idx */
static inline void ft_update(size_t idx, uint32_t delta)
{
    for (size_t i = idx; i < ft_size; i += i & -i)
        ft[i] += delta;
}

/* Helper: sum of positions 1..idx */
static inline uint32_t ft_query(size_t idx)
{
    uint32_t s = 0;
    for (size_t i = idx; i > 0; i -= i & -i)
        s += ft[i];
    return s;
}

/* Modular exponentiation using mul_mod64 for 64‑bit integers. */
static inline uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safe guard for 0, though offset starts at 1 */

    /* Initialise prime list on first call */
    if (!primes_ready)
    {
        build_primes_upto(PRIME_LIMIT, &prime_list, &prime_cnt);
        primes_ready = true;
    }

    /* If n exceeds available primes, return 0 as a defensive fallback */
    if ((size_t)n > prime_cnt) return 0;

    uint64_t p = prime_list[n - 1];           /* prime(n) */
    seq_t m = (seq_t)(p - 1) / 2;             /* number of i's (and j's) */

    if (m <= 1) return 0;                    /* no pairs possible for very small primes */

    /* Set up Fenwick tree size to cover indices up to p (>=1). */
    ft_size = (size_t)p + 1;
    ft_clear(p);                              /* clear necessary portion */

    seq_t inversions = 0;

    for (seq_t i = 1; i <= m; ++i)
    {
        /* Compute val = i^16 mod p */
        uint64_t val = mod_pow64((uint64_t)i, 16ULL, p);
        size_t idx = (size_t)val + 1;          /* 1‑based Fenwick index */

        uint32_t leq   = ft_query(idx);       /* previous elements <= current */
        uint32_t seen  = (uint32_t)(i - 1);    /* number of previous elements */
        inversions += (seq_t)(seen - leq);    /* only those > current contribute */

        ft_update(idx, 1);
    }

    return inversions;
}
