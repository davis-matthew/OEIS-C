/*
 * A367059  G.f. satisfies A(x) = 1 + x*A(x)^3 + x^3*A(x)^3.
 * https://oeis.org/A367059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(2*n-3*k+1,k) *
 *            binomial(3*n-6*k,n-3*k) / (2*n-3*k+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: none – direct summation with multiplicative binomial calculation. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <type_traits>
#endif

/*--------------------------------------------------------------------
 * Multiplicative binomial coefficient.
 * Works for any seq_t: small unsigned integer types or an arbitrary‑precision class.
 *--------------------------------------------------------------------*/
static inline seq_t binom_custom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        idx_t numer = n - k + i;
        idx_t denom = i;

#ifdef __cplusplus
        if (std::is_arithmetic<seq_t>::value)
        {
            unsigned __int128 prod = (unsigned __int128)result * (unsigned __int128)numer;
            result = (seq_t)(prod / denom);
        }
        else
        {
            result = (seq_t)((result * numer) / denom);
        }
#else
        /* In pure C we know seq_t is an unsigned integer type. */
        unsigned __int128 prod = (unsigned __int128)result * (unsigned __int128)numer;
        result = (seq_t)(prod / denom);
#endif
    }
    return result;
}

/*--------------------------------------------------------------------
 * Computes a(n) for the OEIS sequence A367059.
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t u = 2 * n - 3 * k + 1;

        seq_t c1 = binom_custom(u, k);
        seq_t c2 = binom_custom(3 * n - 6 * k, n - 3 * k);

        seq_t term;
#ifdef __cplusplus
        if (std::is_arithmetic<seq_t>::value)
        {
            unsigned __int128 prod = (unsigned __int128)c1 * (unsigned __int128)c2;
            term = (seq_t)(prod / (unsigned __int128)u);
        }
        else
        {
            term = (c1 * c2) / u;
        }
#else
        unsigned __int128 prod = (unsigned __int128)c1 * (unsigned __int128)c2;
        term = (seq_t)(prod / (unsigned __int128)u);
#endif
        sum += term;
    }
    return sum;
}
