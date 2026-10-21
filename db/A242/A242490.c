/*
 * A242490  Smallest even number k such that lpf(k-3) = prime(n) while
 *          lpf(k-1) > lpf(k-3), where lpf=least prime factor (A020639).
 * https://oeis.org/A242490
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest even k such that lpf(k‑3)=prime(n) and lpf(k‑1)
 *            > lpf(k‑3)
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   get_prime, lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/lpf.h"

/* Slow but correct implementation:  for each n≥2 search increasing odd m */
seq_t a(idx_t n)
{
    /* Undefined for n < 2; return 0 for safety */
    if (n < 2) return 0;

    /* nth prime (1‑based) */
    idx_t p = get_prime(n);   /* get_prime returns uint32_t, safe to widen to idx_t */

    /* Search odd multipliers m  (m=1,3,5,…) */
    for (idx_t m = 1; ; m += 2)
    {
        /* Ensure m has no prime factor < p (i.e., lpf(m) ≥ p)  */
        if (m == 1 || lpf(m) >= p)
        {
            /* k = p * m + 3  (k is even because p,m are odd) */
            seq_t k = ((seq_t)p) * m + 3;

            /* k‑1 must be p‑rough: its least prime factor > p */
            idx_t km1 = (idx_t)(k - 1);   /* safe for n up to 10 000 */
            if (lpf(km1) > p)
            {
                return k;
            }
        }
    }
}
