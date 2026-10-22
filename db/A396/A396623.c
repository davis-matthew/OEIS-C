/*
 * A396623  a(n) = r(n,n), where r(1,k)=f(k)=ceiling(k*c), r(n,k)=f(r(n-1,k)),
 *          c=sqrt(3).
 * https://oeis.org/A396623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n, n) where r(1, k) = ceil(k*sqrt(3)) r(n, k) =
 *            ceil(r(n-1, k) * sqrt(3)) over integer n >= 1, k=n. The function
 *            implements the recurrence by applying the step f(x) =
 *            ceil(x*sqrt(3)) exactly using integer arithmetic
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

#if defined(__SIZEOF_INT128__) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
typedef unsigned __int128 u128;
#endif

/* Helper: floor(sqrt(n)) for a 128‑bit unsigned integer. */
#if defined(__SIZEOF_INT128__) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
static seq_t floor_sqrt_128(unsigned __int128 n)
{
    unsigned long long low = 0;
    unsigned long long high = ~0ULL;          /* 2^64-1 */
    while (low <= high) {
        unsigned long long mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return (seq_t)high;
}
#endif

/* Step function: ceil(x * sqrt(3)) */
static seq_t f(seq_t x)
{
#if defined(__SIZEOF_INT128__) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    /* Use 128‑bit arithmetic for the intermediate product */
    uint64_t ux = (uint64_t)x;
    unsigned __int128 prod = (unsigned __int128)3 * ux * ux;
    seq_t s = floor_sqrt_128(prod);
    return s + 1;
#else
    /* Fallback for arbitrary‑precision seq_t; assume the provided
       operators perform correct arithmetic and field floor_sqrt. */
    seq_t tmp = x * x;
    tmp = tmp * 3;
    seq_t s = floor_sqrt(tmp);
    return s + 1;
#endif
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    for (idx_t i = 0; i < n; ++i)
        val = f(val);
    return val;
}
