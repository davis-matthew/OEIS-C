/*
 * A364757  The pyramidal array T(r,g,b) =
 *          (r+g+b)/((g+b)*(r+b))*C(r+g,b-1)*C(g+b,r)*C(r+b,g), where 1 <= b
 *          <= ceiling((r+g+b)/2) and 0 <= r,g <= floor((r+g+b)/2). Read first
 *          over the layers corresponding to fixed sum r+g+b, then over the
 *          diagonals corresponding to fixed b.
 * https://oeis.org/A364757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A364757:
 *  The n-th term is obtained by enumerating the pyramidal array
 *  T(r,g,b) = ((r+g+b)/((g+b)*(r+b))) *
 *             C(r+g,b-1) * C(g+b,r) * C(r+b,g)
 *  where 1 ≤ b ≤ ceil((r+g+b)/2) and 0 ≤ r,g ≤ floor((r+g+b)/2).
 *  The enumeration reads first by increasing sum s = r+g+b,
 *  then by increasing b (the diagonal) inside each layer.
 *  This function returns that a(n) for n ≥ 1.
 */

#include <stddef.h>          /* for size_t */
#include <stdbool.h>         /* for bool */
#include "../utils/binom.h"  /* seq_t binom(idx_t a, idx_t b) */
#include "../utils/gcd.h"    /* seq_t gcd(seq_t a, seq_t b) */

static seq_t compute_T(idx_t r, idx_t g, idx_t b, idx_t s)
{
    /* binomial factors */
    seq_t A = binom(r + g, b - 1);          /* C(r+g,b-1)   */
    seq_t B = binom(g + b, r);              /* C(g+b,r)     */
    seq_t C = binom(r + b, g);              /* C(r+b,g)     */

    /* numerator factors: A, B, C, s */
    seq_t numerators[4] = {A, B, C, (seq_t)s};

    /* denominator factors: (g+b), (r+b) */
    seq_t denom[2] = {g + b, r + b};

    seq_t result = 1;

    for (size_t i = 0; i < 4; ++i) {
        seq_t f = numerators[i];
        /* Reduce this numerator with the denominators */
        for (size_t j = 0; j < 2; ++j) {
            if (denom[j] == 1)
                continue;
            seq_t gval = gcd(f, denom[j]);
            if (gval > 1) {
                f /= gval;
                denom[j] /= gval;
            }
        }
        /* multiply the reduced numerator into the result */
        result *= f;
    }

    /* any remaining denominator factors must divide the result exactly */
    if (denom[0] != 1)
        result /= denom[0];
    if (denom[1] != 1)
        result /= denom[1];

    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset is 1; a(0) is unused */

    idx_t target = n;
    idx_t count  = 0;

    /* Enumerate layers by sum s = r+g+b */
    for (idx_t s = 1; ; ++s) {
        idx_t maxb   = (s + 1) / 2;  /* ceil(s/2)   */
        idx_t maxhalf = s / 2;       /* floor(s/2)  */

        for (idx_t b = 1; b <= maxb; ++b) {
            /* For this b, r+g = s-b */
            if (s < b)           /* should never happen */
                break;
            idx_t delta = s - b; /* r+g = delta */

            /* r ranges from 0 to min(maxhalf, delta) */
            for (idx_t r = 0; r <= maxhalf; ++r) {
                if (r > delta)
                    break;      /* g would be negative */

                idx_t g_val = delta - r;
                if (g_val > maxhalf)
                    continue;    /* g exceeds floor(s/2) */

                /* Valid triple (r,g_val,b) */
                ++count;
                if (count == target)
                    return compute_T(r, g_val, b, s);
            }
        }
    }
}
