/*
 * A293821  Number of integer-sided quadrilaterals having perimeter n, modulo
 *          rotations but not reflections.
 * https://oeis.org/A293821
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (N₀ + 2·N₁ + N₂) / 4 where N₀ = C(n‑1,3) – 4·C(Q,3), Q =
 *            n – ceil(n/2) = floor(n/2), N₁ = 1 if n ≡ 0 (mod 4) else 0, N₂ =
 *            (n/2 – 1) if n even, otherwise 0
 * Range:     u8 n <= 16, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Computed only with integer arithmetic, no floating point or external utilities.) */
#include <stdbool.h>

static inline seq_t binom3(idx_t n)
{
    if (n < 3) return 0;
    /* Compute (n choose 3) = n*(n-1)*(n-2)/6 using 64‑bit arithmetic */
    idx_t v = n * (n - 1) * (n - 2) / 6;
    return (seq_t)v;
}

seq_t a(idx_t n)
{
    /* n >= 4; behaviour for smaller n is irrelevant per specification */

    /* total number of positive quadruples with sum n */
    seq_t total = binom3(n - 1);

    /* compute ceil(n/2) and Q = n - ceil(n/2) = floor(n/2) */
    idx_t half = n >> 1;                     /* floor(n/2)     */
    idx_t ceil_half = (n & 1) ? (half + 1) : half; /* ceil(n/2)  */
    idx_t Q = n - ceil_half;                 /* floor(n/2)     */

    /* number of invalid tuples where a side >= n/2 */
    seq_t invalid = 0;
    if (Q >= 3)
        invalid = 4 * binom3(Q);

    /* N₀ */
    seq_t N0 = total - invalid;

    /* N₁ : all sides equal (requires n divisible by 4) */
    seq_t N1 = ((n & 3) == 0) ? 1 : 0;

    /* N₂ : a = c, b = d, a + b = n/2 (requires n even) */
    seq_t N2 = ( (n & 1) == 0 ) ? ((seq_t)(half - 1)) : 0;

    /* final value: integer division, result always divisible by 4 */
    seq_t numer = N0 + 2 * N1 + N2;
    return numer / 4;
}
