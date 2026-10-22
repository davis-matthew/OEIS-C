/*
 * A308662  Number of ways to write n as (2^a*5^b)^2 + c*(3c+1) + d*(3d+2),
 *          where a and b are nonnegative integers, and c and d are integers.
 * https://oeis.org/A308662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of representations of n as
   (2^a*5^b)^2 + c*(3c+1) + d*(3d+2).
   Enumerates all m = 2^a*5^b with m^2 ≤ n, then for each
   remainder R counts integer pairs (c,d) solving
   R = c*(3c+1) + d*(3d+2).  The count for a given t is found
   by solving the quadratic 3d^2+2d-t=0 using integer
   square‑root tests.  All calculations use only integer
   arithmetic and 128‑bit intermediates when needed. */
#include <stdbool.h>
#include <stdint.h>

/* Integer square root of an unsigned 128–bit value.
   Returns the greatest r such that r^2 ≤ x. */
static idx_t isqrt128(unsigned __int128 x)
{
    idx_t r = 0;
    /* sqrt(x) ≤ 2^64, so 63 bits are enough for the search. */
    for (int i = 63; i >= 0; --i) {
        idx_t bit = ((idx_t)1) << i;
        idx_t cand = r | bit;
        unsigned __int128 sq = (unsigned __int128)cand * cand;
        if (sq <= x)
            r = cand;
    }
    return r;
}

/* Number of integer d such that d*(3d+2) equals the given
   non‑negative t.  The result is 0, 1, or 2. */
static int d_solutions(idx_t t)
{
    unsigned __int128 D = (unsigned __int128)4 + (unsigned __int128)12 * t;
    idx_t s = isqrt128(D);
    if ((unsigned __int128)s * s != D)  /* not a perfect square */
        return 0;
    int cnt = 0;
    /* root d = (s-2)/6 */
    if (s >= 2 && (s - 2) % 6 == 0)
        ++cnt;
    /* root d = -(s+2)/6 */
    if ((s + 2) % 6 == 0)
        ++cnt;
    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t count = 0;

    /* All possible m = 2^a * 5^b with m^2 ≤ n. */
    idx_t sqrt_n = isqrt128(n);           /* floor(sqrt(n)) */
    for (idx_t pow2 = 1; pow2 <= sqrt_n; pow2 <<= 1) {
        for (idx_t pow5 = 1;
             pow2 <= sqrt_n / pow5;
             pow5 *= 5) {
            idx_t m = pow2 * pow5;
            idx_t msq = m * m;            /* msq ≤ n */
            idx_t R  = n - msq;           /* remaining part */

            /* Bounds for |c| from c*(3c+1) ≤ R. */
            unsigned __int128 bound = (unsigned __int128)1 + (unsigned __int128)12 * R;
            idx_t sqrt_term  = isqrt128(bound);
            idx_t k_max_pos = (sqrt_term > 0) ? (sqrt_term - 1) / 6 : 0;
            idx_t k_max_neg = (sqrt_term + 1) / 6;

            /* c = 0 or positive */
            for (idx_t k = 0; k <= k_max_pos; ++k) {
                unsigned __int128 f = (unsigned __int128)k *
                                      ((unsigned __int128)3 * k + 1);
                if ((unsigned __int128)f > (unsigned __int128)R)
                    break;
                idx_t t = R - (idx_t)f;   /* remainder for d */
                int sols = d_solutions(t);
                count += (seq_t)sols;
            }

            /* c = -k, k ≥ 1 */
            for (idx_t k = 1; k <= k_max_neg; ++k) {
                unsigned __int128 f = (unsigned __int128)k *
                                      ((unsigned __int128)3 * k - 1);
                if ((unsigned __int128)f > (unsigned __int128)R)
                    break;
                idx_t t = R - (idx_t)f;
                int sols = d_solutions(t);
                count += (seq_t)sols;
            }
        }
    }

    return count;
}
