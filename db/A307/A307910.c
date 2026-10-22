/*
 * A307910  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of 1/sqrt(1 - 2*k*x + k*(k-4)*x^2).
 * https://oeis.org/A307910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 145, u64 n <= 368,
 *            u128 n <= 975, big any n
 * Helpers:   antidiagonal_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: square array A(n,k) read by antidiagonals,
 * with A(0,k)=1, A(1,k)=k, and recurrence
 * n*A(n,k)=k*(2n‑1)*A(n‑1,k) – k*(k‑4)*(n‑1)*A(n‑2,k).
 * For k<4 the second term is negative, so we add the absolute value.
 * This implementation uses 128‑bit intermediates when seq_t is an
 * integral type to avoid overflow, and a direct recurrence otherwise.
 */
#include "../utils/antidiagonal_coords.h"
#ifdef __cplusplus
# include <type_traits>
#endif

static seq_t compute_A(idx_t i, idx_t j)
{
    /* base cases */
    if (i == 0) return 1;
    if (i == 1) return j;

    seq_t prev2 = 1;       /* A(0,j) */
    seq_t prev1 = j;       /* A(1,j) */

    for (idx_t n = 2; n <= i; ++n)
    {
#ifdef __cplusplus
        /* If seq_t is an integral type, use 128‑bit intermediates. */
        if constexpr (std::is_integral<seq_t>::value)
        {
            unsigned __int128 num1 = (unsigned __int128)j *
                                    (unsigned __int128)(2 * n - 1) *
                                    (unsigned __int128)prev1;

            unsigned __int128 num2 = (unsigned __int128)j *
                                    (unsigned __int128)(j >= 4 ? j - 4 : 4 - j) *
                                    (unsigned __int128)(n - 1) *
                                    (unsigned __int128)prev2;

            unsigned __int128 numer = j >= 4 ? num1 - num2 : num1 + num2;

            prev2 = prev1;
            prev1 = (seq_t)(numer / n);
        }
        else
        {
            /* seq_t is a big‑integer type */
            seq_t term1 = j * ((seq_t)(2 * n - 1)) * prev1;
            seq_t term2 = j * ((seq_t)(j >= 4 ? j - 4 : 4 - j)) *
                          ((seq_t)(n - 1)) * prev2;
            seq_t numer = j >= 4 ? term1 - term2 : term1 + term2;

            prev2 = prev1;
            prev1 = numer / n;
        }
#else
        /* C implementation – assume seq_t is an unsigned integer type. */
        unsigned __int128 num1 = (unsigned __int128)j *
                                 (unsigned __int128)(2 * n - 1) *
                                 (unsigned __int128)prev1;
        unsigned __int128 num2 = (unsigned __int128)j *
                                 (unsigned __int128)(j >= 4 ? j - 4 : 4 - j) *
                                 (unsigned __int128)(n - 1) *
                                 (unsigned __int128)prev2;
        unsigned __int128 numer = j >= 4 ? num1 - num2 : num1 + num2;

        prev2 = prev1;
        prev1 = (seq_t)(numer / n);
#endif
    }

    return prev1;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return compute_A(i, j);
}
