/*
 * A354196  a(n) = A064989(A064989(sigma(sigma(A003961(A003961(n)))))), where
 *          A003961 shifts the prime factorization of n one step towards
 *          larger primes, and A064989 shifts it back towards smaller primes.
 * https://oeis.org/A354196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   factor_u64, next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A064989(A064989(sigma(sigma(A003961(A003961(n))))))
 *
 * The function implements the A003961 and A064989 transformations
 * (shift prime factorizations up and down respectively), the sum-of-divisors
 * function sigma, and applies the sequence transformation.
 *
 * The implementation uses 64‑bit arithmetic for all intermediate
 * operations; results that fit in seq_t are cast back to seq_t.
 * For n up to 10\,000 all values comfortably fit in 64 bits.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"

/* Compute p^e using 128‑bit accumulator, return uint64_t (wraparound is OK). */
static inline uint64_t pow_uint64(uint64_t p, unsigned e)
{
    unsigned __int128 r = 1;
    unsigned __int128 b = p;
    while (e)
    {
        if (e & 1) r *= b;
        b *= b;
        e >>= 1;
    }
    return (uint64_t)r;
}

/* Compute p^e using 128‑bit accumulator, return unsigned __int128. */
static inline unsigned __int128 pow_uint128(uint64_t p, unsigned e)
{
    unsigned __int128 r = 1;
    unsigned __int128 b = p;
    while (e)
    {
        if (e & 1) r *= b;
        b *= b;
        e >>= 1;
    }
    return r;
}

/* Sum‑of‑divisors function sigma for uint64_t. */
static inline uint64_t sigma_uint64(uint64_t n)
{
    if (n <= 1) return 1;  /* sigma(1)=1 */

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    unsigned __int128 res = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = pr[i];
        unsigned e = (unsigned)ex[i];
        /* (p^(e+1) - 1) / (p - 1) */
        unsigned __int128 term = (pow_uint128(p, e + 1) - 1) / (p - 1);
        res *= term;
    }
    return (uint64_t)res;
}

/* Shift prime factors of n up (next prime for each factor). */
static inline uint64_t shift_up_uint64(uint64_t n)
{
    if (n <= 1) return n;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    unsigned __int128 res = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = pr[i];
        unsigned e = (unsigned)ex[i];
        uint64_t q = next_prime(p);
        unsigned __int128 term = pow_uint128(q, e);
        res *= term;
    }
    return (uint64_t)res;
}

/* Shift prime factors of n down (previous prime for each factor; 2 → 1). */
static inline uint64_t shift_down_uint64(uint64_t n)
{
    if (n <= 1) return 1;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    unsigned __int128 res = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = pr[i];
        unsigned e = (unsigned)ex[i];
        uint64_t q;
        if (p == 2)
            q = 1;                  /* previous prime of 2 is treated as 1 */
        else
            q = prev_prime(p);
        if (q <= 1) continue;      /* multiplicand is 1 */
        unsigned __int128 term = pow_uint128(q, e);
        res *= term;
    }
    return (uint64_t)res;
}

/* Main sequence function: a(n) = A064989(A064989(sigma(sigma(A003961(A003961(n))))) ) */
seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;          /* cast index to 64‑bit */
    uint64_t y = shift_up_uint64(x);
    uint64_t z = shift_up_uint64(y);
    uint64_t s = sigma_uint64(z);
    uint64_t t = sigma_uint64(s);
    uint64_t u = shift_down_uint64(t);
    uint64_t v = shift_down_uint64(u);
    return (seq_t)v;
}
