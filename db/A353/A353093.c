/*
 * A353093  Solution to Forest of Numbers (Bosque de Números) puzzle for
 *          Opaque Queens starting with numbers 1 and 2 for an n X n square
 *          grid (see Comments).
 * https://oeis.org/A353093
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = floor((n^2 + 2) / 3) + f(n mod 6), where f = {4, 3, 1, 3,
 *            4, 4} for mod 6 values 0..5
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n)
{
    /*  The sequence is defined only for n >= 2.  For n < 2 the value
        is unspecified; we return 0 in this case. */
    if (n < (idx_t)2)
        return (seq_t)0;

    /* Compute floor((n^2 + 2) / 3).  We calculate n*n in seq_t to avoid
       overflow for large n and then perform the addition and division. */
    seq_t nsq = (seq_t)n * (seq_t)n;              /* n^2      */
    seq_t base = (nsq + (seq_t)2) / (seq_t)3;    /* floor((n^2+2)/3) */

    /* Adjustment depending on n mod 6. */
    static const int adjust[6] = {4, 3, 1, 3, 4, 4};
    int mod6 = (int)(n % (idx_t)6);
    base += (seq_t)adjust[mod6];

    return base;
}
