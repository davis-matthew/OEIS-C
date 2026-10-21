/*
 * A257390  Number of 4-Motzkin paths of length n with no level steps at even
 *          level.
 * https://oeis.org/A257390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((-16*n + 40)*a(n-3) + (-12*n+12)*a(n-2) +
 *            (8*n+4)*a(n-1))/(n+2) a(0) = 1, a(1) = 0, a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* helper that multiplies a seq_t value by a small integer using built‑in
   arithmetic when possible, otherwise uses the native * operator */
static inline seq_t mul_by_seq_small(seq_t val, idx_t mul)
{
    return val * (seq_t)mul;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)0;   /* a(n-2)   */
    seq_t a2 = (seq_t)1;   /* a(n-1)   */
    seq_t a3 = a2;         /* current value */

    for (idx_t k = 3; k <= n; ++k)
    {
#ifdef __cplusplus
        /* C++: use a compile‑time check to select the implementation that
           works for the actual type of seq_t. */
        if constexpr (std::is_integral<seq_t>::value &&
                      sizeof(seq_t) <= sizeof(unsigned long long))
        {
            /* Built‑in integer type – use 128‑bit intermediate to avoid
               overflow. */
            __uint128_t t1 = (__uint128_t)(8*k + 4) * (unsigned long long)a2;
            __uint128_t t2 = (__uint128_t)(16*k - 40) * (unsigned long long)a0;
            __uint128_t t3 = (__uint128_t)(12*k - 12) * (unsigned long long)a1;
            __uint128_t numer = t1 - t2 - t3;
            a3 = (seq_t)(numer / (k + 2));
        }
        else
        {
            /* Arbitrary‑precision seq_t – use native multiplication. */
            seq_t t1 = mul_by_seq_small(a2, 8*k + 4);
            seq_t t2 = mul_by_seq_small(a0, 16*k - 40);
            seq_t t3 = mul_by_seq_small(a1, 12*k - 12);
            seq_t numer = t1 - t2 - t3;
            a3 = numer / (k + 2);
        }
#else
        /* C implementation – assume 128‑bit width exists for the compiler
           and seq_t is an unsigned integer type.  This covers the native
           uint*_t builds.  For arbitrary‑precision builds the compiler is
           expected to use C++ mode. */
        __uint128_t t1 = (__uint128_t)(8*k + 4) * (unsigned long long)a2;
        __uint128_t t2 = (__uint128_t)(16*k - 40) * (unsigned long long)a0;
        __uint128_t t3 = (__uint128_t)(12*k - 12) * (unsigned long long)a1;
        __uint128_t numer = t1 - t2 - t3;
        a3 = (seq_t)(numer / (k + 2));
#endif
        /* shift the sliding window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a3;
}
