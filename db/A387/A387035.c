/*
 * A387035  a(n) = Sum_{k=0..n} binomial(4*n-3,k).
 * https://oeis.org/A387035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n-3,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#if defined(__cplusplus)
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;            /* a(0) = 1 */

    /* Compute N = 4*n - 3 */
    idx_t N = 4 * n - 3;

    seq_t sum = (seq_t)1;          /* starts with C(N,0) */
    seq_t C   = (seq_t)1;          /* current binomial value */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update C = C * (N - (k-1)) / k  while staying within the limits
           of the underlying integer type. */
#if defined(__cplusplus)
        if constexpr (std::is_integral<seq_t>::value) {
            /* seq_t is a built‑in unsigned integer: use __int128 to avoid
               overflow while the intermediate result still fits in 128 bits. */
            unsigned __int128 tmp = (unsigned __int128)C * (N - (k - 1));
            C = (seq_t)(tmp / k);
        } else {
            /* seq_t is an arbitrary‑precision type: built‑in multiplication
               and division are exact and do not overflow. */
            C = C * (seq_t)(N - (k - 1));
            C = C / (seq_t)k;
        }
#else
        /* In plain C we assume seq_t is an unsigned integer; use
           128‑bit intermediate to avoid overflow. */
        unsigned __int128 tmp = (unsigned __int128)C * (N - (k - 1));
        C = (seq_t)(tmp / k);
#endif
        sum += C;                 /* accumulate the binomial term */
    }

    return sum;
}
