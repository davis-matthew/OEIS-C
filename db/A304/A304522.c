/*
 * A304522  Number of ordered ways to write n as the sum of a Fibonacci number
 *          and a positive odd squarefree number.
 * https://oeis.org/A304522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of ordered representations n = F + S, where F is a
 *            Fibonacci number (value, not index) ≤ n and S is a positive odd
 *            square‑free integer. Duplicate Fibonacci values (the two 1's in
 *            the standard sequence) are counted only once, matching the OEIS
 *            definition
 * Range:     u8 n <= 1795, u16 n <= 9785, u32 n <= 9785, u64 n <= 9785,
 *            u128 n <= 9785, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t f_prev = 0;   /* current Fibonacci value */
    seq_t f_curr = 1;   /* next Fibonacci value */
    bool one_seen = false; /* ensures the value 1 is used only once */

    while (f_prev <= (seq_t)n) {
        /* Skip the second occurrence of the value 1 */
        if (!(f_prev == 1 && one_seen)) {
            seq_t diff = (seq_t)n - f_prev;
            if (diff > 0 && (diff & 1) && is_squarefree(diff))
                count++;
            if (f_prev == 1)
                one_seen = true;
        }

        /* Advance to the next Fibonacci number */
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;
    }
    return count;
}
