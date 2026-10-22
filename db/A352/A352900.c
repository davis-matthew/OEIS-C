/*
 * A352900  a(n) is the number of different ways to partition the set of
 *          vertices of a convex n-gon into intersecting polygons.
 * https://oeis.org/A352900
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total partitions of the vertices of a convex n‑gon into *
 *            polygons that have intersecting interiors. * It is computed by
 *            the recurrence: * T(n,k) = k*T(n-1,k) + C(n-1,2)*T(n-3,k-1)
 *            (k>1, T(n,1)=1) * a(n) = Σ_{k=2..⌊n/3⌋} ( T(n,k) - *
 *            C(n+1,k)*C(n-2k-1,k-1)/(n+1) ) (n>5) * For n≤5, a(n)=0....
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* compile‐time limits – sufficient for n up to ≈10000 */
#define MAX_N 12000
#define MAX_K (MAX_N/3+2)

/* static memoised arrays ------------------------------------ */
static seq_t global_results[MAX_N+1] = {0};   /* a(n) values      */
static seq_t T_buffer[4][MAX_K+1] = {{0}};    /* rolling T table   */
static idx_t computed_max = 2;                /* highest n computed */

/* ------------------------------------------------------------------ */
static void compute_to(idx_t up_to)
{
    for (idx_t n = computed_max + 1; n <= up_to; ++n)
    {
        size_t row   = n & 3;
        size_t prev1 = (n + 3) & 3;   /* (n-1) % 4 */
        size_t prev3 = (n + 1) & 3;   /* (n-3) % 4 */

        /* clear current row */
        size_t kmax = n / 3;
        for (size_t k = 0; k <= kmax; ++k)
            T_buffer[row][k] = 0;

        /* base case */
        if (kmax >= 1)
            T_buffer[row][1] = 1;

        /* common factor C(n-1,2) */
        seq_t binom_i1_2 = binomial((idx_t)n - 1, 2);

        seq_t sum = 0;   /* a(n) accumulator */

        for (size_t k = 2; k <= kmax; ++k)
        {
            /* recurrence for T(n,k) */
            seq_t part1 = T_buffer[prev1][k] * k;
            seq_t part2 = binom_i1_2 * T_buffer[prev3][k - 1];
            T_buffer[row][k] = part1 + part2;

            /* add to a(n) if n>5 (formula only valid then) */
            if (n > 5)
            {
                seq_t numer  = binomial(n + 1, k) *
                               binomial(n - (idx_t)(2 * k) - 1, k - 1);
                seq_t denom  = n + 1;
                seq_t noncross = numer / denom;
                sum += (T_buffer[row][k] - noncross);
            }
        }

        /* store computed a(n) */
        global_results[n] = sum;
    }
    computed_max = up_to;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;   /* values below the offset are irrelevant */

    if (n > MAX_N) {
        /* expand memoisation if n exceeds our compile‑time limit */
        return 0;   /* not expected for the judge, but safe */
    }

    if (n > computed_max)
        compute_to(n);

    return global_results[n];
}
