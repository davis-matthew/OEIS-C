/*
 * A380511  Expansion of e.g.f. exp(x*G(x)^2) where G(x) = 1 + x*G(x)^3 is the
 *          g.f. of A001764.
 * https://oeis.org/A380511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * n! * sum_{k=0}^{n-1} binomial(2*n + k, k) / * ((2*n +
 *            k) * (n-k-1)!) * Recurrence used for efficient evaluation: * let
 *            U_0 = 1 * for k = 0 .. n-2: * U_{k+1} = U_k * (2*n + k) * (n - k
 *            - 1) / (k + 1) * a(n) = sum_{k=0}^{n-1} U_k * This recurrence
 *            keeps all intermediate values integral
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    seq_t U   = 1;          /* U_0 */
    sum += U;

    /* Compute subsequent U_k terms. */
    for (idx_t k = 0; k + 1 < n; ++k)
    {
        const seq_t num1 = (seq_t)(2 * (seq_t)n + (seq_t)k);
        const seq_t num2 = (seq_t)(n - k - 1);
        seq_t       den  = (seq_t)(k + 1);
        seq_t       t    = U;

        /* Reduce (den, num1) */
        seq_t g = gcd(den, num1);
        den /= g;
        seq_t a1 = num1 / g;

        /* Reduce (den, num2) */
        g = gcd(den, num2);
        den /= g;
        seq_t a2 = num2 / g;

        /* Reduce (t, den) */
        g = gcd(t, den);
        t /= g;
        den /= g;   /* should become 1 */

        /* Compute next U. The division by den is exact. */
        seq_t U_next = t * a1 * a2;
        if (den != 1)
            U_next /= den;  /* safety: in a correctly derived recurrence den==1 */

        U = U_next;
        sum += U;
    }

    return sum;
}
