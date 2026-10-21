/*
 * A276008  Substitute ones for all nonzero digits in factorial base
 *          representation of n: a(n) = A059590(A275727(n)).
 * https://oeis.org/A276008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret the factorial‑base representation of n * with
 *            all nonzero digits replaced by 1 (i.e. a(n) =
 *            A059590(A275727(n)))
 * Range:     u8 n <= 297, u16 n <= 9466, u32 n <= 9466, u64 n <= 9466,
 *            u128 n <= 9466, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic. */
    seq_t temp = (seq_t)n;
    seq_t result = 0;       /* value of a(n) */
    seq_t factorial = 1;    /* k! for current position k (starting at 1) */
    idx_t k = 1;            /* factorial‑base position */

    /* Extract factorial‑base digits from least significant (1!) upward. */
    while (temp > 0)
    {
        seq_t divisor = (seq_t)(k + 1); /* next factorial base limit */
        seq_t digit = temp % divisor;   /* current digit in [0, k] */
        if (digit != 0)
            result += factorial;        /* replace by 1, so add k! */
        temp /= divisor;                /* remove processed part */
        factorial *= divisor;           /* next factorial ( (k+1)! ) */
        k++;
    }

    return result;
}
