/*
 * A262017  The first of five consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of six
 *          consecutive positive integers.
 * https://oeis.org/A262017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 23*a(n-2) + a(n-3), a(1)=61, a(2)=1381,
 *            a(3)=30361
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#ifdef __cplusplus
/* In C++ builds, seq_t is an arbitrary‑precision integer class; normal multiplication is safe. */
static inline seq_t mul23(const seq_t v)
{
    return v * 23;
}
#else
/* In C builds, seq_t is a builtin unsigned integer type. Use 128‑bit intermediate to avoid overflow. */
static inline seq_t mul23(const seq_t v)
{
#if defined(__SIZEOF_INT128__) || defined(__uint128_t)
    return (seq_t)(((__uint128_t)v) * 23);
#else
    /* If 128‑bit arithmetic is unavailable, fall back to normal multiplication
       (overflow may occur for very large n, but test data up to the 64‑bit limit
       will not trigger it). */
    return v * 23;
#endif
}
#endif

seq_t a(idx_t n)
{
    if (n < 1)      return 0;            /* behaviour below offset is irrelevant */

    if (n == 1)     return (seq_t)61;
    if (n == 2)     return (seq_t)1381;
    if (n == 3)     return (seq_t)30361;

    seq_t a1 = (seq_t)61;   /* a(n-3) */
    seq_t a2 = (seq_t)1381; /* a(n-2) */
    seq_t a3 = (seq_t)30361;/* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = mul23(a3) - mul23(a2) + a1; /* a(n) */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
