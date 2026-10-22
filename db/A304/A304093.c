/*
 * A304093  a(n) is the number of the proper divisors of n that are Lucas
 *          numbers (A000204, with 2 excluded).
 * https://oeis.org/A304093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of proper divisors of n that are Lucas numbers
 *            (excluding 2)
 * Range:     u8 n <= 291, u16 n <= 8324, u32 n <= 8324, u64 n <= 8324,
 *            u128 n <= 8324, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    if (n_seq <= 1)
        return 0;

    seq_t count = 0;
    seq_t a_luc = (seq_t)2;
    seq_t b_luc = (seq_t)1;

    /* 2 is the first Lucas number but must be ignored.  The second Lucas number 1 counts as a divisor of all n. */
    if (a_luc <= n_seq && a_luc != n_seq && a_luc != (seq_t)2) {
        if (n_seq % a_luc == 0)
            count++;
    }
    if (b_luc <= n_seq && b_luc != n_seq) {
        if (n_seq % b_luc == 0)
            count++;
    }

    /* Generate further Lucas numbers until we exceed n_seq. */
    while (1) {
        seq_t c_luc = a_luc + b_luc;
        if (c_luc > n_seq)
            break;
        if (c_luc != n_seq) {
            if (n_seq % c_luc == 0)
                count++;
        }
        a_luc = b_luc;
        b_luc = c_luc;
    }

    return count;
}
