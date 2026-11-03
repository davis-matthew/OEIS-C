/*
 * A347904  Array read by antidiagonals, m, n >= 1: T(m,n) is the first prime
 *          (after the two initial terms) in the Fibonacci-like sequence with
 *          initial terms m and n, or 0 if no such prime exists.
 * https://oeis.org/A347904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,n). T(m,n) is the first prime that appears after the
 *            two initial terms in the Fibonacci‑like sequence with initial
 *            terms m and n (read by antidiagonals, m,n≥1). If m and n are not
 *            coprime or if no prime ever appears, T(m,n)=0
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-03)
 * Helpers:   gcd, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static inline void antidiagonal_coords_calc(idx_t n, idx_t *i, idx_t *j)
{
    /* Find diagonal d s.t. d(d+1)/2 ≥ n, with d≥1. */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) {
        ++d;
    }
    idx_t prev = (d - 1) * d / 2;
    idx_t t = n - prev;            /* position within the diagonal (1‑based) */
    *i = t;                        /* m value */
    *j = d - t + 1;                /* n value */
}

static seq_t compute_T(seq_t m, seq_t n)
{
    if (gcd(m, n) != (seq_t)1)
        return 0;  /* initial terms not coprime → no prime */

    seq_t a = m, b = n;
    /* Search for the first prime in the generated sequence.  The number of
       terms needed is extremely small in practice, but we set a generous
       upper bound to avoid infinite loops in pathological cases. */
    for (size_t iter = 0; iter < 20000; ++iter) {
        seq_t s = a + b;
        if (isPrimeGeneric(s))
            return s;
        a = b;
        b = s;
    }
    return 0;  /* did not find a prime within the bound */
}

seq_t a(idx_t n)
{
    if (n == 0)   /* OEIS offset is 1 */
        return 0;

    idx_t i, j;
    antidiagonal_coords_calc(n, &i, &j);
    seq_t m = (seq_t)i;
    seq_t k = (seq_t)j;
    return compute_T(m, k);
}
