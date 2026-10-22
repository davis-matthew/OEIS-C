/*
 * A358064  Expansion of e.g.f. 1/(1 - x * exp(x^2)).
 * https://oeis.org/A358064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n! * Sum_{k=0..⌊n/2⌋} (n-2k)^k / k!
 * (computed directly from the definition)
 */
#include <stdint.h>            /* for uint64_t in case it is useful */

static seq_t pow_seq_small(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1U)
            result *= base;
        base *= base;
        exp >>= (idx_t)1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* compute n!  */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t result = (seq_t)0;
    seq_t cur_fact = fact;                     /* n! / k! for current k   */
    idx_t half = n >> 1;                       /* floor(n/2)              */

    for (idx_t k = (idx_t)0; k <= half; ++k)
    {
        seq_t base  = (seq_t)(n - (idx_t)2 * k);
        seq_t power = pow_seq_small(base, k);
        result += cur_fact * power;
        /* advance cur_fact to n! / (k+1)!  (exact division) */
        cur_fact /= (seq_t)(k + (idx_t)1);
    }
    return result;
}
