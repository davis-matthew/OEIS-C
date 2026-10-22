/*
 * A337355  a(n) is the denominator of Product_{i=0..n-1}
 *          (n-i)^((-1)^ceiling(i/2)).
 * https://oeis.org/A337355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 36, u64 n <= 76, u128 n <= 151,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 * Sequence A337355
 * a(n) = denominator of  ∏_{i=0}^{n-1} (n-i)^{(-1)^⌈i/2⌉}
 * The function below computes this denominator exactly for every n ≥ 1.
 * It uses only integer arithmetic; for large values it works as a big‑
 * integer if the build defines seq_t as such.
 *********************************************************************/

#include <stdint.h>
#include <stdlib.h>

#include "../utils/pow_int.h"   /* pow_int(seq_t base, idx_t exp) */

/* Function: a
 *  Computes the denominator of the product described above.
 *  For n < 1 it returns 1 as a safe default.
 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)1;           /* undefined index, return 1 */

    /* Array of net prime exponents  (value >0 → numerator, <0 → denominator) */
    int *exp = (int *)calloc((size_t)(n + 1), sizeof(int));
    if (!exp)
        return (seq_t)0;            /* allocation failure (unlikely) */

    /* ------------------------------------------------------------------
     * Main loop: factor each j = n, n-1, … , 1 according to the sign
     * of its exponent (+1 or –1).  The sign is +1 when ⌈i/2⌉ is even,
     * where i = n‑j.
     * ------------------------------------------------------------------ */
    for (uint64_t j = n; j >= 1; --j) {
        uint64_t i    = n - j;                     /* i = n - j  */
        uint64_t half = (i + 1) / 2;               /* ceil(i/2)  */
        int      sign = (half & 1) ? -1 : +1;      /* parity of ceil(i/2) */

        uint64_t temp = j;
        for (uint64_t p = 2; p * p <= temp; ++p) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            if (cnt)
                exp[p] += sign * cnt;
        }
        if (temp > 1) {                      /* remaining prime factor */
            exp[temp] += sign;                /* exponent is ±1 */
        }
    }

    /* ------------------------------------------------------------------
     * Build the denominator from the negative exponents.
     * ------------------------------------------------------------------ */
    seq_t denominator = (seq_t)1;
    for (uint64_t p = 2; p <= (uint64_t)n; ++p) {
        int e = exp[p];
        if (e < 0) {
            seq_t mul = pow_int((seq_t)p, (idx_t)(-e));
            denominator *= mul;              /* * works for both int/BigInt */
        }
    }

    /* Clean-up ---------------------------------------------------------- */
    free(exp);

    return denominator;
}
