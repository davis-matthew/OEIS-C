/*
 * A344600  a(n) = Sum_{k=1..n} phi(k) * (floor(n/k)^4 - floor((n-1)/k)^4).
 * https://oeis.org/A344600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} phi(k) * (floor(n/k)^4 - floor((n-1)/k)^4)
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation computes the totient array phi[1..n] by a standard
    sieve (O(n log log n)), then evaluates the summand for each k.  All
    arithmetic is performed in the type seq_t, using only the operators
    allowed by the problem statement.  The function is thread‑safe and
    fully self‑contained except for the helpers defined in the utils
    directory. */

#include <stdlib.h>
#include "../utils/seq_mul.h"

/* --------------------------------------------------------------- */
/*  Helper: compute phi[1..n]  ----------------------------------- */
/* --------------------------------------------------------------- */
static void compute_phi(idx_t n, seq_t *phi)
{
    /* phi[i] initially equals i */
    for (idx_t i = 1; i <= n; ++i)
        phi[i] = (seq_t)i;

    for (idx_t i = 2; i <= n; ++i)
    {
        if (phi[i] == (seq_t)i)          /* i is prime */
        {
            for (idx_t j = i; j <= n; j += i)
            {
                /* phi[j] = phi[j] / i * (i-1) */
                seq_t tmp = phi[j] / i;
                phi[j] = seq_mul(tmp, (seq_t)(i - 1));
            }
        }
    }
}

/* --------------------------------------------------------------- */
/*  Main sequence routine  --------------------------------------- */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* safety for n < 1, although not needed */

    /* Allocate storage for phi[1..n] */
    seq_t *phi = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!phi)
        return (seq_t)0;        /* out of memory – unlikely for the expected n */

    compute_phi(n, phi);

    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* floor divisions */
        idx_t m  = n  / k;
        idx_t m1 = (n - 1) / k;

        /* compute m^4 and m1^4 */
        seq_t m_s  = (seq_t)m;
        seq_t m1_s = (seq_t)m1;

        seq_t m_sq = seq_mul(m_s, m_s);
        seq_t m1_sq = seq_mul(m1_s, m1_s);
        seq_t m4  = seq_mul(m_sq, m_sq);
        seq_t m1_4 = seq_mul(m1_sq, m1_sq);

        /* difference */
        seq_t diff = m4 - m1_4;

        /* accumulate phi(k) * diff */
        seq_t prod = seq_mul(phi[k], diff);
        sum += prod;
    }

    free(phi);
    return sum;
}
