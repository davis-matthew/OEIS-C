/*
 * A394525  a(n) is the length of the finite sequence of positive integers
 *          (x_1, x_2, ...), where x_1 = n and, for i >= 1, x_{i+1} = max { k
 *          > 0 | x_i + k^2 is a perfect square } if such a k exists;
 *          otherwise x_i is the last term of the sequence.
 * https://oeis.org/A394525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A394525: Length of the chain defined by
   x₁ = n, xᵢ₊₁ = max{ k>0 | xᵢ + k² is a perfect square }.
   Recurrence: for a current value cur,
       if cur == 1 or cur == 2 or cur == 4 or cur ≡ 2 (mod 4) → stop.
       else if cur is odd           → next = (cur - 1) / 2.
       else (cur is divisible by 4) → next = (cur - 4) / 4.
   Count the terms including the starting term.  */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined, return 0 */

    seq_t cur = (seq_t)n;
    seq_t len = 1;                      /* include the first term */

    while (true) {
        /* Root conditions: 1, 2, 4 or 2 (mod 4) */
        if (cur == 1 || cur == 2 || cur == 4 || (cur & 3) == 2)
            break;

        if (cur & 1) {                  /* odd */
            cur = (cur - 1) / 2;
        } else {                        /* divisible by 4 */
            cur = (cur - 4) / 4;
        }
        ++len;
    }

    return len;
}
