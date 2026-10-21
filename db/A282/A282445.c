/*
 * A282445  For n>=5, a(n) is the smallest m>=3 such that odd part of
 *          ((prime(n)^2 - prime(m)^2)/3) is prime, or a(n)=0 if there is no
 *          such m<n.
 * https://oeis.org/A282445
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest m ≥ 3 such that the odd part of
 *            ((prime(n)^2 – prime(m)^2)/3) is prime; a(n)=0 if no such m<n
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   get_prime, isPrimeGeneric, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/odd_part.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n < 5U)
        return 0;              /* undefined for n < 5, return 0 for safety */

    seq_t p_n = (seq_t)get_prime(n);   /* nth prime */

    for (idx_t m = 3U; m < n; ++m)     /* search m from 3 up to n-1 */
    {
        seq_t p_m = (seq_t)get_prime(m);
        seq_t diff = p_n * p_n - p_m * p_m;   /* (p_n² – p_m²) */
        seq_t val  = diff / 3;                /* divide by 3 */
        seq_t odd  = odd_part(val);           /* odd part of val */

        if (isPrimeGeneric(odd))
            return (seq_t)m;                  /* first such m */
    }

    return 0;   /* no suitable m found */
}
