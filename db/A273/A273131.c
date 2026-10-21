/*
 * A273131  Numbers n such that the bottom entry of the difference table of
 *          the divisors of n divides n.
 * https://oeis.org/A273131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 37, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A273131:  all powers of two together with the fixed exceptions
 *  6, 12, 14, 24, 152.  The function merges the two sorted lists. */
#include <stddef.h>
#include <stdint.h>

static const seq_t EXCEPTIONS[] = { (seq_t)6, (seq_t)12, (seq_t)14, (seq_t)24, (seq_t)152 };
static const size_t NUM_EXCEPTIONS = sizeof(EXCEPTIONS) / sizeof(EXCEPTIONS[0]);

seq_t a(idx_t n)
{
    idx_t count   = 0;          /* number of terms emitted so far */
    seq_t power   = 1;           /* current power of two to output */
    size_t ex_idx = 0;           /* index into the exception list */

    while (count < n)
    {
        seq_t next_val;
        if (ex_idx < NUM_EXCEPTIONS && EXCEPTIONS[ex_idx] <= power)
        {
            next_val = EXCEPTIONS[ex_idx++];
        }
        else
        {
            next_val = power;
            power <<= 1;         /* next power of two */
        }
        ++count;
        if (count == n)
            return next_val;
    }
    return (seq_t)0;            /* never reached */
}
