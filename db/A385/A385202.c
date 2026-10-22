/*
 * A385202  Irregular triangle read by rows: let S be an ordered set of
 *          nondivisors of n such that a and b belong to S if a + b = n.
 *          T(n,k) is the k-th member of S. If S is empty, T(n,k) = 0.
 * https://oeis.org/A385202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385202: irregular triangle read by rows.
   For each n = 1,2,3,...
   Define S(n) = { a | 1 ≤ a < n, a ∤ n and (n-a) ∤ n } .
   Output all elements of S(n) in ascending order; if S(n) is empty,
   output a single 0.  The sequence consists of all these outputs
   concatenated by rows.  The function below returns the n-th term
   in this concatenated sequence. */

#include <stdbool.h>
#include <stddef.h>

extern seq_t a(idx_t n)
{
    idx_t target = n;            /* 1‑based position requested */
    idx_t pos = 1;               /* current position in the output sequence */

    for (idx_t r = 1; ; ++r) {   /* iterate over rows n = r */
        bool empty = true;

        for (idx_t a = 1; a < r; ++a) {
            /* a is a non‑divisor of r and r-a is also a non‑divisor */
            if (r % a == 0 || r % (r - a) == 0) {
                continue;
            }
            /* include a in the triangle */
            if (pos == target) {
                return (seq_t)a;
            }
            ++pos;
            empty = false;
        }

        /* if the row is empty, output a single 0 */
        if (empty) {
            if (pos == target) {
                return (seq_t)0;
            }
            ++pos;
        }
    }
}
