/*
 * A243517  Number of length n+2 0..8 arrays with no three elements in a row
 *          with pattern aba or abb (with a!=b) and new values 0..8 introduced
 *          in 0..8 order.
 * https://oeis.org/A243517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A243517: number of length n+2 0..8 arrays with no three
 * consecutive elements forming pattern aba or abb (a≠b) and new values
 * introduced in order 0..8.
 *
 * Recurrence (valid for n >= 8):
 *   a(1)=3, a(2)=5, a(3)=10, a(4)=25, a(5)=77, a(6)=280, a(7)=1157
 *   a(n) = 23*a(n-1) - 212*a(n-2) + 1010*a(n-3)
 *           - 2669*a(n-4) + 3887*a(n-5) - 2878*a(n-6) + 840*a(n-7)
 *
 * The implementation uses a sliding window of the last seven terms.
 * All operations are performed on the type seq_t, which is a sufficiently
 * large unsigned integer type (possibly a big‑int class).  The
 * subtraction is safe because the recurrence guarantees the result is
 * non‑negative for all n ≥ 8.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* constants for the first seven terms */
    static const seq_t init[7] = {
        (seq_t)3,   /* a(1) */
        (seq_t)5,   /* a(2) */
        (seq_t)10,  /* a(3) */
        (seq_t)25,  /* a(4) */
        (seq_t)77,  /* a(5) */
        (seq_t)280, /* a(6) */
        (seq_t)1157 /* a(7) */
    };

    if (n <= 7)
        return init[n - 1];

    /* sliding window: last[0] = a(k-7), ..., last[6] = a(k-1) */
    seq_t last[7];
    for (size_t i = 0; i < 7; ++i)
        last[i] = init[i];

    /* compute a(8) up to a(n) */
    for (idx_t k = 8; k <= n; ++k) {
        seq_t sum_pos = (seq_t)23  * last[6] +
                        (seq_t)1010 * last[4] +
                        (seq_t)3887 * last[2] +
                        (seq_t)840  * last[0];

        seq_t sum_neg = (seq_t)212  * last[5] +
                        (seq_t)2669 * last[3] +
                        (seq_t)2878 * last[1];

        seq_t new_val = sum_pos - sum_neg; /* guaranteed non‑negative */

        /* shift window left and insert new_val at the end */
        for (size_t j = 0; j < 6; ++j)
            last[j] = last[j + 1];
        last[6] = new_val;
    }

    return last[6]; /* a(n) */
}
