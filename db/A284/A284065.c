/*
 * A284065  Numbers whose smallest decimal digit is 4.
 * https://oeis.org/A284065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   power6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS entry describes the sequence of all positive integers whose
   least decimal digit equals 4.  The function below returns the nth such
   integer (offset 1).  It does not use a pre‑computed table; instead it
   constructs numbers digit by digit by counting how many admissible
   completions exist for each possible leading digit.  The counting
   uses only integer arithmetic on the types seq_t and idx_t supplied
   by the build harness. */

#include <stdbool.h>
#include "../utils/power6.h"

static inline seq_t power5(idx_t exp) {
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r *= 5;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* defensive: offset is 1 */

    /* Step 1: find the number of digits L of the answer */
    seq_t cumulative = 0;              /* count of admissible numbers up to length L */
    seq_t cumulative_prev = 0;         /* count up to length L-1 */
    idx_t L = 0;

    while (1) {
        ++L;
        seq_t count_len = power6(L) - power5(L);  /* numbers of length L with at least one 4 */
        cumulative_prev = cumulative;
        cumulative += count_len;
        if (cumulative >= n) break;
    }

    /* Step 2: determine the 0‑based offset r among the length‑L numbers */
    seq_t r = (seq_t)n - cumulative_prev - 1;

    /* Step 3: construct the r‑th number of length L */
    seq_t result = 0;
    bool have4 = false;

    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t remaining = L - pos - 1;
        seq_t pow6_rem = power6(remaining);
        seq_t pow5_rem = power5(remaining);

        for (int d = 4; d <= 9; ++d) {
            bool new_have4 = have4 || (d == 4);
            seq_t count = new_have4 ? pow6_rem : (pow6_rem - pow5_rem);

            if (r < count) {
                result = result * 10 + (seq_t)d;
                have4 = new_have4;
                break;
            } else {
                r -= count;
            }
        }
    }

    return result;
}
