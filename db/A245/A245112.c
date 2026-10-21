/*
 * A245112  G.f. A(x) satisfies A(x)^2 = 1 + 4*x*A(x)^5.
 * https://oeis.org/A245112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A245112
   Recurrence:  a(0) = 1,  a(1) = 2
   a(n) = a(n-2) * 20*(5*n-9)*(5*n-3)*(5*n-7)*(5*n-1)
                        / (3*n*(n-1)*(3*n-1)*(3*n+1))
   After each term the result is an integer.  The implementation
   reduces common factors step‑by‑step to avoid overflow on 64‑bit
   builds.  It uses only the functions provided by the framework
   (gcd).  For arbitrary‑precision seq_t the same operations work
   because the big‑integer type overloads the usual operators.  */
#include "../utils/gcd.h"

static inline seq_t compute_next(idx_t k, seq_t prev)
{
    /* Denominator factors: 3, k, k-1, 3*k-1, 3*k+1 */
    seq_t den[5] = { (seq_t)3,
                     (seq_t)k,
                     (seq_t)(k-1),
                     (seq_t)(3*k-1),
                     (seq_t)(3*k+1) };
    /* Numerator factors: 20, 5*k-9, 5*k-3, 5*k-7, 5*k-1 */
    seq_t num[5] = { (seq_t)20,
                     (seq_t)(5*k-9),
                     (seq_t)(5*k-3),
                     (seq_t)(5*k-7),
                     (seq_t)(5*k-1) };

    seq_t r = prev;

    for (idx_t i = 0; i < 5; ++i)
    {
        /* Cancel common factors between the current numerator
           factor and all remaining denominator factors. */
        for (idx_t j = 0; j < 5; ++j)
        {
            seq_t g = gcd(num[i], den[j]);
            if (g > 1)
            {
                num[i] /= g;
                den[j] /= g;
            }
        }

        /* Multiply the reduced numerator factor into the running
           result.  The intermediate product may be larger than
           seq_t, but for arbitrary‑precision types this is fine,
           and for 64‑bit types the previous reductions keep it
           within range. */
        r *= num[i];

        /* Remove any common factors between the current result
           and the remaining denominator factors. */
        for (idx_t j = 0; j < 5; ++j)
        {
            seq_t g = gcd(r, den[j]);
            if (g > 1)
            {
                r /= g;
                den[j] /= g;
            }
        }
    }

    /* Any remaining denominator factors should now be 1.
       This division is safe and has no effect if all are 1. */
    for (idx_t j = 0; j < 5; ++j)
    {
        if (den[j] > 1)
            r /= den[j];
    }

    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    if (n & 1)        /* odd n */
    {
        seq_t prev = (seq_t)2;   /* a(1) */
        for (idx_t k = 3; k <= n; k += 2)
            prev = compute_next(k, prev);
        return prev;
    }
    else              /* even n */
    {
        seq_t prev = (seq_t)1;   /* a(0) */
        for (idx_t k = 2; k <= n; k += 2)
            prev = compute_next(k, prev);
        return prev;
    }
}
