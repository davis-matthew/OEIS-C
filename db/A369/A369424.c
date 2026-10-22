/*
 * A369424  Expansion of Sum_{n>=1} x^(n^2)*((1+x)/(1-x))^n.
 * https://oeis.org/A369424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{m=1}^{⌊√n⌋} Σ_{t=0}^{min(m, n-m²)} binom(m, t) *
 *            binom(m + (n-m²) – t – 1, m‑1)
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 96, u64 n <= 262,
 *            u128 n <= 758, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This expands the series Σ_{k≥1} x^{k²}((1+x)/(1‑x))^k as required.  */

#include <stdint.h>

/* seq_t and idx_t are defined by the build.  We rely only on
   integer arithmetic (+, -, *, /, %) that is defined for seq_t.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;              /* offset is 1; values below unused */

    seq_t total = 0;

    /* Loop over m = 1 … ⌊√n⌋.  Use s <= n / s to avoid overflow. */
    for (idx_t s = 1; s <= n / s; ++s)
    {
        idx_t rem = n - s * s;                /* n - m²  */
        idx_t i_limit = (rem < s ? rem : s);   /* min(s, rem) */

        /* Compute binom(s, 0 … i_limit) incrementally in currB.  */
        seq_t currB = 1;                 /* binom(s, 0) */

        /* Compute binom(s + rem – 1, s‑1) as the starting value for C2.  */
        seq_t currC2 = 1;                 /* binom(s‑1, s‑1) */
        for (idx_t j = 1; j <= rem; ++j)
        {
            /* binom(s + j – 1, s‑1) = binom(s + j – 2, s‑1) * (s + j – 1) / j  */
            currC2 = currC2 * (seq_t)(s + j - 1) / (seq_t)j;
        }

        /* Accumulate contributions for this s.  */
        for (idx_t i = 0; i <= i_limit; ++i)
        {
            total += currB * currC2;

            if (i == i_limit)
                break;        /* no more i; stop updating */

            /* Update binomial binom(s, i+1) */
            currB = currB * (seq_t)(s - i) / (seq_t)(i + 1);

            /* Update binomial binom(s + rem – i – 1, s‑1) to the next k = rem – (i+1) */
            idx_t k = rem - i;                       /* current km = rem – i   */
            /* binom(s + k – 1, s‑1) = binom(s + k – 2, s‑1) * k / (s + k – 1) */
            currC2 = currC2 * (seq_t)k / (seq_t)(s + k - 1);
        }
    }

    return total;
}
