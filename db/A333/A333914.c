/*
 * A333914  a(n) is the least integer m >= 3 such that n is m-gonal pyramidal
 *          number.
 * https://oeis.org/A333914
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m ≥ 3 such that n is an m‑gonal pyramidal number.
 *            N(m,k) = k(k+1)(k(m-2)+5‑m)/6. The function examines all
 *            feasible k and finds the minimal m
 * Range:     u8 n <= 34, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n < 4 we are outside the OEIS offset; return 3 as a safe value. */
    if (n < 4u)
        return (seq_t)3;

    seq_t best_m = 0;          /* minimal m found so far */
    bool found   = false;      /* has any candidate been found? */
    seq_t six_n  = (seq_t)6 * (seq_t)n;   /* 6 * n */

    for (idx_t k = 2; ; ++k)
    {
        seq_t kk = (seq_t)k;

        /* N(3,k) = k(k+1)(k+2)/6; stop when it exceeds n. */
        seq_t n3 = kk * (kk + 1) * (kk + 2) / 6;
        if (n3 > (seq_t)n)
            break;          /* all larger k give N(3,k) > n → stop */

        seq_t C = kk * (kk + 1);          /* k(k+1) */

        seq_t numerator, denom;
        if (k == 2)            /* special case: 2k‑5 = -1  → subtraction */
        {
            numerator = six_n - C;         /* 6n - C */
            denom     = C;                 /* C*(k-1) with k‑1 == 1 */
        }
        else
        {
            /* 2k‑5 is non‑negative for k ≥ 3 */
            seq_t term = C * ( (seq_t)(2ULL * k - 5ULL) );   /* C*(2k‑5) */
            numerator = six_n + term;                        /* 6n + C*(2k‑5) */
            denom     = C * (kk - 1);                        /* C*(k‑1) */
        }

        if (numerator % denom != 0)
            continue;       /* m would not be an integer for this k */

        seq_t m = numerator / denom;
        if (m >= (seq_t)3 && (!found || m < best_m))
        {
            best_m = m;
            found  = true;
        }
    }

    /* For any n ≥ 4 a solution always exists: k=2 gives m = n‑1. */
    return best_m;
}
