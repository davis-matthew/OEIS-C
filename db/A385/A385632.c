/*
 * A385632  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(5*n+1,k).
 * https://oeis.org/A385632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * binomial(5*n+1,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(0) + Sum_{k=1..n} T_k where
   T_0 = 2^n,
   T_k = T_{k-1} * (5n+1-k+1) / (k*2). */

#include "../utils/pow2.h"   /* pow2(idx_t)          */
#include "../utils/gcd.h"    /* gcd(seq_t, seq_t)   */

/* Helper for safe multiplication.
   In C++ we use constexpr and type traits;
   in plain C we rely on 128‑bit arithmetic for integral seq_t. */
#ifdef __cplusplus
#include <type_traits>
inline seq_t mul_term_safe(seq_t a, unsigned long long b)
{
    if constexpr (std::is_integral<seq_t>::value) {
        unsigned __int128 tmp = (unsigned __int128)a * b;
        return (seq_t)tmp;
    } else {
        /* seq_t is an arbitrary‑precision class; the * operator handles the work. */
        return a * (seq_t)b;
    }
}
#else
/* Assume seq_t is an unsigned integer type. */
static inline seq_t mul_term_safe(seq_t a, unsigned long long b)
{
    unsigned __int128 tmp = (unsigned __int128)a * b;
    return (seq_t)tmp;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* m = 5*n + 1 */
    seq_t m = (seq_t)5 * (seq_t)n + (seq_t)1;

    seq_t sum  = (seq_t)0;
    seq_t term = pow2(n);          /* 2^n  */
    sum += term;                   /* add T_0 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t numerator = m - (seq_t)(k - 1);  /* m - (k-1) */
        seq_t denom     = (seq_t)k * (seq_t)2; /* k*2          */

        /* Reduce common factors to keep intermediate values small. */
        seq_t g = gcd(term, denom);
        term  /= g;
        denom /= g;

        g = gcd(numerator, denom);
        numerator /= g;
        denom     /= g;

        /* term = term * numerator / denom  (exact division). */
        term = mul_term_safe(term, (unsigned long long)numerator);
        term = term / denom;

        sum += term;
    }
    return sum;
}
