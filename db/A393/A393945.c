/*
 * A393945  Number of binary words of length n with an even number of
 *          occurrences of subword 101.
 * https://oeis.org/A393945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 * a(0) = 1, a(1) = 2
 * For n ≥ 2, a(n) counts binary words of length n with an even
 * number of 101 substrings.  The calculation uses an automaton
 * with states (last two bits, parity of occurrences). */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* even[2bit], odd[2bit] hold counts for current length
     * (initialized for length 2). */
    seq_t even[4] = {1, 1, 1, 1};
    seq_t odd[4]  = {0, 0, 0, 0};

    seq_t newEven[4];
    seq_t newOdd[4];

    for (idx_t len = 3; len <= n; ++len) {
        /* reset new arrays to zero */
        for (int i = 0; i < 4; ++i) {
            newEven[i] = 0;
            newOdd[i] = 0;
        }

        for (int old = 0; old < 4; ++old) {
            seq_t cntE = even[old];
            if (cntE) {
                for (int bit = 0; bit < 2; ++bit) {
                    int newLast = ((old << 1) | bit) & 3;
                    if (old == 2 && bit == 1) {      /* 10 + 1 forms 101 */
                        newOdd[newLast] += cntE;     /* parity flips */
                    } else {
                        newEven[newLast] += cntE;     /* parity stays even */
                    }
                }
            }

            seq_t cntO = odd[old];
            if (cntO) {
                for (int bit = 0; bit < 2; ++bit) {
                    int newLast = ((old << 1) | bit) & 3;
                    if (old == 2 && bit == 1) {
                        newEven[newLast] += cntO;     /* parity flips to even */
                    } else {
                        newOdd[newLast] += cntO;      /* parity stays odd */
                    }
                }
            }
        }

        /* copy new arrays back into current state arrays */
        for (int i = 0; i < 4; ++i) {
            even[i] = newEven[i];
            odd[i]  = newOdd[i];
        }
    }

    /* result is sum of counts in even-parity states */
    seq_t result = 0;
    for (int i = 0; i < 4; ++i) result += even[i];
    return result;
}
