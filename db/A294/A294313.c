/*
 * A294313  Expansion of e.g.f. sech(x*exp(x)).
 * https://oeis.org/A294313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{m=0}^{⌊n/2⌋} E_{2m} * binom(n,2m) * (2m)^{n-2m} where
 *            E_{2m} are Euler numbers with E0=1, E2=-1, E4=5,... a(0)=1,
 *            a(1)=0, a(2)=-1, … (as in OEIS A294313)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return 1;

    /* ----- 1.  Euler numbers E_{2m} (stored as euler[m] = E_{2m}) ----- */
    static seq_t *euler = NULL;
    static size_t euler_len = 0;   /* number of cached entries */

    idx_t m_max = n / 2;

    /* initialise cache if needed */
    if (euler_len == 0) {
        euler = (seq_t *)malloc(sizeof(seq_t));
        if (!euler) return 0;          /* out‑of‑memory fallback */
        euler[0] = 1;                  /* E0 = 1 */
        euler_len = 1;
    }

    /* extend cache to include E_{2m} up to m_max */
    if (euler_len <= (size_t)m_max) {
        size_t new_len = (size_t)(m_max + 1);
        seq_t *tmp = (seq_t *)realloc(euler, new_len * sizeof(seq_t));
        if (!tmp) return 0;            /* out‑of‑memory fallback */
        euler = tmp;

        for (idx_t m = euler_len; m <= m_max; ++m) {
            seq_t sum = 0;
            for (idx_t k = 0; k < m; ++k) {
                seq_t t = binomial(2 * m, 2 * k);
                t = t * euler[k];
                sum = sum + t;
            }
            euler[m] = -sum;          /* recurrence: E_{2m} = -Σ_{k<m} binom(2m,2k)E_{2k} */
        }
        euler_len = new_len;
    }

    /* ----- 2.  Compute a(n) ----- */
    seq_t result = 0;
    for (idx_t m = 1; m <= m_max; ++m) {   /* m=0 contributes only to a(0) */
        seq_t bin  = binomial(n, 2 * m);
        seq_t pow  = seq_pow((seq_t)(2 * m), n - 2 * m);
        seq_t term = euler[m] * bin;
        term = term * pow;
        result = result + term;
    }
    return result;
}
