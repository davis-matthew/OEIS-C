/*
 * A319886  a(n) = 3*2*1 - 6*5*4 + 9*8*7 - 12*11*10 + 15*14*13 - 18*17*16 +
 *          ... - (up to the n-th term).
 * https://oeis.org/A319886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 4*a(n-3) + 4*a(n-4) - 6*a(n-6) + 6*a(n-7) -
 *            4*a(n-9) + 4*a(n-10) - a(n-12) + a(n-13) for n > 13 Initial
 *            values (offset 1): a(1) = 3, a(2) = 6, a(3) = 6, a(4) = 0, a(5)
 *            = -24, a(6) = -114, a(7) = -105, a(8) = -42, a(9) = 390, a(10) =
 *            378, a(11) = 258, a(12) = -930, a(13) = -915 Thi...
 * Range:     i8 n <= 8, i16 n <= 41, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for NULL, if needed */

seq_t a(idx_t n)
{
    /* Precomputed first 13 terms */
    static const seq_t init[13] = {
        (seq_t)3,   (seq_t)6,   (seq_t)6,   (seq_t)0,
        (seq_t)-24, (seq_t)-114, (seq_t)-105, (seq_t)-42,
        (seq_t)390, (seq_t)378, (seq_t)258, (seq_t)-930,
        (seq_t)-915
    };

    if (n <= 13) {
        /* Direct lookup for the initial segment */
        return init[n - 1];
    }

    /* Buffer of the last 13 computed values.
       buf[0] = a(i‑13), ..., buf[12] = a(i‑1) when the loop index i is current.  */
    seq_t buf[13];
    for (idx_t i = 0; i < 13; ++i) {
        buf[i] = init[i];
    }

    for (idx_t i = 14; i <= n; ++i) {
        /* Apply the linear recurrence */
        seq_t val = buf[12];              /* a(i‑1)   */
        val -= (seq_t)4 * buf[10];       /* -4*a(i‑3) */
        val += (seq_t)4 * buf[9];        /* +4*a(i‑4) */
        val -= (seq_t)6 * buf[7];        /* -6*a(i‑6) */
        val += (seq_t)6 * buf[6];        /* +6*a(i‑7) */
        val -= (seq_t)4 * buf[4];        /* -4*a(i‑9) */
        val += (seq_t)4 * buf[3];        /* +4*a(i‑10) */
        val -= buf[1];                   /* -a(i‑12) */
        val += buf[0];                   /* +a(i‑13) */

        /* Shift buffer left to make room for the new value */
        for (int j = 0; j < 12; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[12] = val;
    }

    /* At this point buf[12] holds a(n) */
    return buf[12];
}
