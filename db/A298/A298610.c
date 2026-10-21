/*
 * A298610  Triangle read by rows, the unsigned coefficients of G(n, n, x/2)
 *          where G(n,a,x) denotes the n-th Gegenbauer polynomial, T(n, k) for
 *          0 <= k <= n.
 * https://oeis.org/A298610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A298610: Triangle read by rows of the unsigned coefficients of
 * the Gegenbauer polynomial G(n,n,x/2).
 *
 * For a row r and column k (0 ≤ k ≤ r) let
 *   m = (r-k)/2          (integer because r-k is even for non‑zero entries)
 *   t = (3*r + k)/2
 * Then the coefficient (non‑zero only when r-k is even) is a multinomial:
 *
 *       a(r,k) = (t-1)! / ((r-1)! * m! * k!)
 *              = binomial(t-1, r-1) * binomial(t-r, m)
 *
 * The sequence a(n) is obtained by flattening the triangle by rows.
 *
 * The function below finds the row and column for the given linear
 * index n, tests the parity condition, and computes the value using
 * the binomial helper functions.  It uses only integer arithmetic
 * and the utility functions provided for arbitrary‑precision types. */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* n = 0 gives the first term 1 */
    if (n == 0u)
        return (seq_t)1;

    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
       The loop runs O(r) ≈ O(√n) steps – fast enough for the required limits. */
    idx_t r = 0;
    while (1) {
        __int128 val = (__int128)(r + 1) * (r + 2) / 2;
        if (val > n)
            break;
        r++;
    }

    /* Column index within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* If r-k is odd the coefficient is zero */
    if (((r - k) & 1u) != 0u)
        return (seq_t)0;

    /* For the remaining cases r ≥ 1 and r-k even */
    idx_t m = (r - k) / 2;
    idx_t t = (3 * r + k) / 2;

    /* Compute the multinomial coefficient using binomials */
    seq_t part1 = binomial(t - 1, r - 1);
    seq_t part2 = binomial(t - r, m);
    seq_t result = seq_mul(part1, part2);

    return result;
}
