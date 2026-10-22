/*
 * A343386  Number of odd Motzkin n-paths, i.e., Motzkin n-paths with an odd
 *          number of up steps.
 * https://oeis.org/A343386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   acc_add, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A343386: Number of odd Motzkin n-paths (OEIS) */
/* Computation via the explicit formula
   a(n) = Σ_{k odd, 0≤k≤⌊n/2⌋} C(n,2k) * Catalan(k)
         C denotes binomial coefficient, Catalan(k) is A000108(k)
   For n<5 the first values are hard‑coded. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/catalan.h"
#include "../utils/acc_add.h"

static seq_t binomial(unsigned long long n, unsigned long long r)
{
    seq_t result = 1;
    for (unsigned long long i = 1; i <= r; ++i) {
        result = result * (n - i + 1) / i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Offset 0, a(0)=0, a(1)=0, a(2)=1, a(3)=3, a(4)=6 */
    if (n == 0 || n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 3;
    if (n == 4) return 6;

    /* Compute using the explicit sum */
    seq_t binom = 1;                 /* C(n,0) */
    seq_t acc_mag = 0;               /* magnitude of accumulated sum S */
    int acc_sign = +1;               /* sign of acc (+1 or -1) */

    for (unsigned long long r = 1; r <= (unsigned long long)n; ++r) {
        binom = binom * (n - r + 1) / r;   /* C(n,r) */
        if ((r & 1) == 0) {                /* r even -> candidate k=r/2 */
            unsigned long long k = r >> 1;
            if ((k & 1) == 1) {            /* k odd -> contributes */
                seq_t coeff = binom * catalan(k);
                /* coefficient sign positive, but we need to subtract
                   because recurrence is a(n) = sum_{odd k} C(n,2k) * Catalan(k)
                   and we are using the explicit formula. */
                acc_add(&acc_mag, &acc_sign, coeff, +1);
            }
        }
    }
    /* The above accumulates Σ_{odd k} C(n,2k)*Catalan(k). That's a(n). */
    return acc_mag;
}
