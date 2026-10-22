/*
 * A397279  Numbers whose binary expansion is of the form 1^i 0^j 1^i 0^j with
 *          i,j >= 1.
 * https://oeis.org/A397279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl, easy
 * Formula:   a(n) = 2^j * (2^i-1) * (2^s+1) * where s =
 *            ceil((1+sqrt(8n+1))/2), * i = n - C(s-1,2), * j = s - i
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1.  a(1) is the first listed term. */
    if (n == 0) return 0;          /* safety: not used for valid indices */

    /* ----- compute s = ceil((1 + sqrt(8n+1))/2) ----- */
    seq_t eight_n = (seq_t)8 * (seq_t)n;
    seq_t sqroot  = floor_sqrt((seq_t)1 + eight_n);   /* floor sqrt(1+8n) */
    idx_t s = (idx_t)(((seq_t)1 + sqroot) / 2);       /* initial floor estimate */

    /* ensure that C(s,2) >= n */
    while ( ((seq_t)(s-1) * (seq_t)s) / 2 < (seq_t)n )
        ++s;

    /* ----- compute i and j ----- */
    seq_t prev = ((seq_t)(s-1) * (seq_t)(s-2)) / 2;   /* C(s-1,2) */
    idx_t i = (idx_t)((seq_t)n - prev);               /* 1 <= i < s */
    idx_t j = s - i;                                  /* j >= 1 */

    /* ----- evaluate 2^i, 2^j, 2^s ----- */
    seq_t pow_i = pow2(i);
    seq_t pow_j = pow2(j);
    seq_t pow_s = pow2(s);

    /* ----- assemble the term ----- */
    seq_t term1 = pow_i - 1;          /* 2^i - 1   */
    seq_t term2 = pow_s + 1;          /* 2^s + 1   */
    seq_t result = pow_j * term1 * term2;

    return result;
}
