/*
 * A284067  Numbers whose smallest decimal digit is 6.
 * https://oeis.org/A284067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose smallest decimal digit is 6 (A284067) */

#include <stdint.h>
#include <stdbool.h>

/* Compute base^exp (base and result are seq_t, exp is idx_t).  */
static seq_t power_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

/* Return the n‑th number whose smallest decimal digit is 6. */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;   /* offset is 1, but handle n==0 safely */
    }

    const seq_t n_seq = (seq_t)n;
    idx_t len = 1;
    seq_t cumulative = (seq_t)0;

    /* Find the digit length that contains the nth number. */
    for (;;) {
        seq_t count_len = power_seq((seq_t)4, len) - power_seq((seq_t)3, len);
        if (cumulative + count_len >= n_seq) {
            break;
        }
        cumulative += count_len;
        ++len;
    }

    /* Zero‑based rank within numbers of the found length. */
    seq_t r = n_seq - cumulative - (seq_t)1;

    seq_t result = (seq_t)0;
    bool has6 = false;   /* whether a 6 has already appeared in the prefix */

    for (idx_t pos = 0; pos < len; ++pos) {
        idx_t rem = len - pos - 1;
        seq_t pow4rem = power_seq((seq_t)4, rem);
        seq_t pow3rem = power_seq((seq_t)3, rem);

        for (int digit = 6; digit <= 9; ++digit) {
            seq_t cnt;
            if (digit == 6) {          /* putting a 6 guarantees the property */
                cnt = pow4rem;
            } else {
                cnt = has6 ? pow4rem : (pow4rem - pow3rem);
            }

            if (r < cnt) {
                result = result * (seq_t)10 + (seq_t)digit;
                if (digit == 6) {
                    has6 = true;
                }
                break;               /* move to the next position */
            } else {
                r -= cnt;
            }
        }
    }

    return result;
}
