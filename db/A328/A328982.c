/*
 * A328982  Sorted list of the numbers of the form 5m+2 (m>=0) together with
 *          numbers of the form 5m-2+eps (m>=1), where eps = 1 if the binary
 *          expansion of m ends in an odd number of 0's and is otherwise 0.
 * https://oeis.org/A328982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A328982:
 * a(n) = sorted union of {5m+2 | m≥0} and {5m–2+eps(m) | m≥1},
 * where eps(m)=1 if the binary representation of m ends in an odd
 * number of 0s, otherwise 0. */
#include <stdint.h>
#include <stdbool.h>

/* Count trailing zeros of a positive integer x. */
static idx_t trailing_zeros(idx_t x)
{
    idx_t tz = 0;
    while ((x & 1ULL) == 0ULL) {
        tz++;
        x >>= 1;
    }
    return tz;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n is always >= 1 in proper use */

    idx_t count = 0;
    idx_t m1 = 0;   /* index for values 5m+2, m ≥ 0 */
    idx_t m2 = 1;   /* index for values 5m–2+eps(m), m ≥ 1 */
    seq_t value = 0;

    while (count < n) {
        seq_t val1 = (seq_t)5 * (seq_t)m1 + (seq_t)2;
        idx_t tz = trailing_zeros(m2);
        seq_t val2 = (seq_t)5 * (seq_t)m2 - (seq_t)2
                     + ((tz & 1ULL) ? (seq_t)1 : (seq_t)0);

        if (val1 < val2) {
            value = val1;
            m1++;
        } else {
            value = val2;
            m2++;
        }
        count++;
    }
    return value;
}
