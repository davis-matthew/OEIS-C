/*
 * A246086  Paradigm shift sequence for (1,4) production scheme with
 *          replacement.
 * https://oeis.org/A246086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 113, u32 n <= 245, u64 n <= 507,
 *            u128 n <= 1000, big any n
 * Helpers:   base_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A246086
 *  Computation:
 *  For n <= 31 the sequence follows simple incremental rules
 *  (1‑1 for 2 ≤ n ≤ 18, 2‑1 for 19 ≤ n ≤ 21, 3‑1 for 22 ≤ n ≤ 29,
 *   4‑1 for 30 ≤ n ≤ 31).
 *  For n ≥ 32 the recurrence a(n) = 3 · a(n − 13) holds.
 *  The function returns a(0) = 0 (undefined for the offset).
 */
#include <stdint.h>
#include "../utils/base_value.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is undefined below the offset */

    /* For n ≥ 32 use the recurrence a(n) = 3 · a(n−13).  */
    if (n >= 32) {
        idx_t m   = n;          /* copy of n that will be decremented */
        idx_t mult = 0;         /* number of times we multiply by 3   */
        while (m >= 32) {
            m   -= 13;
            mult++;
        }
        seq_t val = base_value(m);   /* base value for the reduced index */

        /* multiply by 3^mult */
        for (idx_t i = 0; i < mult; ++i)
            val *= (seq_t)3;
        return val;
    }

    /* n < 32 : compute directly by the incremental rules */
    return base_value(n);
}
