/*
 * A322450  Number of permutations of [2n] in which the size of the last cycle
 *          is n and the cycles are ordered by increasing smallest elements.
 * https://oeis.org/A322450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)!/(n*(n+1))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t res = (seq_t)1;   /* a(1) */

    for (idx_t k = 2; k <= n; ++k)
    {
        /* term = 2*k*(2*k-1)*(k-1)  (fits in a 64‑bit integer) */
        seq_t term = (seq_t)(2*k) * (seq_t)(2*k-1) * (seq_t)(k-1);

#ifdef __cplusplus
        /* For integral seq_t use a 128‑bit product to avoid overflow. */
        if constexpr (std::is_integral<seq_t>::value) {
            unsigned __int128 prod = (unsigned __int128)res * (unsigned __int128)term;
            prod /= (unsigned long long)(k+1);
            res = (seq_t)prod;
        } else {
            /* Non‑integral seq_t (arbitrary precision) – let operator overloads handle the math */
            res = (res * term) / (seq_t)(k+1);
        }
#else
#  ifdef __SIZEOF_INT128__
        /* C implementation with 128‑bit intermediate (most compilers that support it). */
        res = (seq_t)(((unsigned __int128)res * (unsigned __int128)term) / (k+1));
#  else
        /* Fallback – relies on seq_t being large enough that the product fits. */
        res = (res * term) / (seq_t)(k+1);
#  endif
#endif
    }
    return res;
}
