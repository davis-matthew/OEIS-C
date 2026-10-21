/*
 * A283611  Numbers whose largest decimal digit is 8.
 * https://oeis.org/A283611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of sequence A283611.
   a(n) is the n‑th positive integer whose largest decimal digit equals 8.
   The algorithm counts how many such numbers have at most L digits (9^L–8^L),
   finds the digit length L of the n‑th number, then constructs the
   number digit by digit using combinatorial counts. */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1U)                 /* the offset is 1; undefined otherwise */
        return (seq_t)0U;

    /* Find the minimal number of decimal digits L such that
       9^L – 8^L  >=  n   (cumulative count up to L digits).       */
    idx_t pow9  = 1U;   /* 9^0 */
    idx_t pow8  = 1U;   /* 8^0 */
    idx_t cumulative = 0U;
    idx_t L = 0U;

    while (cumulative < n) {
        ++L;
        pow9  *= 9U;
        pow8  *= 8U;
        cumulative = pow9 - pow8;          /* 9^L – 8^L  */
    }

    /* Pre‑compute powers of 9 and 8 up to L for quick lookup. */
    idx_t pow9tab[64];
    idx_t pow8tab[64];
    pow9tab[0] = 1U;
    pow8tab[0] = 1U;
    for (idx_t i = 1U; i <= L; ++i) {
        pow9tab[i] = pow9tab[i - 1] * 9U;
        pow8tab[i] = pow8tab[i - 1] * 8U;
    }

    /* Index of the desired number among length-`L` numbers (0‑based). */
    idx_t prev_cumulative = (pow9tab[L - 1] - pow8tab[L - 1]);  /* 9^(L‑1) – 8^(L‑1) */
    idx_t idx_in_L = n - prev_cumulative - 1U;

    /* Construct the number digit by digit. */
    idx_t value = 0U;
    int has8 = 0;                     /* flag whether ‘8’ has appeared yet */

    for (idx_t pos = 0U; pos < L; ++pos) {
        size_t rem = (size_t)(L - pos - 1U);          /* remaining positions */
        int d_min = (pos == 0U) ? 1 : 0;              /* no leading zero */

        for (int d = d_min; d <= 8; ++d) {
            int next_has8 = has8 || (d == 8);
            idx_t total = pow9tab[rem];              /* 9^rem */

            idx_t count;
            if (next_has8) {
                count = total;
            } else {
                count = total - pow8tab[rem];        /* 9^rem – 8^rem */
            }

            if (idx_in_L < count) {
                value = value * 10U + (idx_t)d;
                has8 = next_has8;
                break;
            } else {
                idx_in_L -= count;
            }
        }
    }

    return (seq_t)value;
}
