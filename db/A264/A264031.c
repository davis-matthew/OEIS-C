/*
 * A264031  Minimum of the sum r + s of the coefficients of a linear
 *          combination of consecutive squares r*k^2 + s*(k+1)^2 equals to n,
 *          with r, s and k >=0.
 * https://oeis.org/A264031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation of A264031:
    a(n) = minimum r + s such that n = r * k² + s * (k+1)²,  r,s,k ≥ 0.
    The algorithm searches over k, computes the maximal feasible
    s using modular arithmetic, and keeps the smallest r+s. */
#include <stdint.h>
#include <stdbool.h>

/* return the inverse of b modulo a (a ≥ 1, b and a coprime).
   Since a is small (≤ 10⁴ in the range where the function is used),
   a brute‑force search is fast and avoids big‑integer operations. */
static idx_t modinv(idx_t b, idx_t a)
{
    if (a == 1) return 0;           /* any s satisfies the congruence   */
    for (idx_t x = 1; x < a; ++x) { /* find x with b·x ≡ 1 (mod a)      */
        if ((b * x) % a == 1) return x;
    }
    return 0;                       /* unreachable for coprime a,b        */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; 0 is unused    */
    idx_t best = n;                /* representation 0·k² + n·1²         */

    /* iterate over k with k² ≤ n   */
    for (idx_t k = 1; k <= n / k; ++k)
    {
        idx_t a_coef = k * k;                 /* k²          */
        idx_t b_coef = (k + 1) * (k + 1);     /* (k+1)²       */

        idx_t inv = modinv(b_coef, a_coef);   /* inverse of b mod a */
        idx_t s0;

        if (a_coef == 1)                     /* special case k = 1 */
            s0 = 0;
        else
            s0 = ((n % a_coef) * inv) % a_coef;

        idx_t max_s = n / b_coef;
        if (max_s < s0) continue;            /* no admissible s for this k */

        idx_t t   = (max_s - s0) / a_coef;
        idx_t s   = s0 + t * a_coef;
        idx_t r   = (n - s * b_coef) / a_coef;
        idx_t sum = r + s;

        if (sum < best) best = sum;
    }
    return (seq_t)best;
}
