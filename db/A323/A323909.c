/*
 * A323909  Balanced ternary representation of A004718, Per Nørgård's
 *          "infinity sequence".
 * https://oeis.org/A323909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained from A004718(n) by writing it in balanced
 *            ternary, replacing each digit −1 with 2, and evaluating the
 *            resulting digits as a base‑3 number. The recurrence of A004718
 *            is A004718(0) = 0 A004718(2k) = −A004718(k) A004718(2k+1) =
 *            A004718(k) + 1 This implementation follows that recurre...
 * Range:     u8 n <= 9458, u16 n <= 9458, u32 n <= 9458, u64 n <= 9458,
 *            u128 n <= 9458, big any n
 * Helpers:   _a004718
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_a004718.h"

/* Main sequence function required by the evaluation harness. */
seq_t a(idx_t n)
{
    /* 1.  Evaluate the integer A004718(n). */
    int64_t x = _a004718(n);

    /* 2.  Special case: balanced ternary of 0 is empty (value 0). */
    if (x == 0)
        return (seq_t)0;

    /* 3.  Convert the integer to balanced ternary and immediately
       accumulate the base‑3 evaluation of the mapped digits. */
    seq_t result = 0;        /* accumulator for the final value  */
    seq_t pow3   = 1;        /* current weight 3^k                  */
    int64_t v    = x;        /* working copy of the operand       */

    while (v != 0) {
        /* a balanced‑ternary digit r ∈ {−2,−1,0,1,2} follows from
           the remainder of v divided by 3. */
        int r = v % 3;
        v   /= 3;

        /* Adjust when the remainder is 2 or −2. */
        if (r == 2) {          /* 2 → −1 with a carry          */
            r = -1;
            v += 1;
        } else if (r == -2) {  /* −2 → 1 with a borrow        */
            r = 1;
            v -= 1;
        }

        /* Map the balanced digit to the target digit:
           −1 → 2, 0 → 0, 1 → 1. */
        unsigned char d = (r == -1) ? 2U : (unsigned char)r;

        /* Add the contribution of this digit. */
        result += (seq_t)d * pow3;
        pow3   *= 3;          /* advance to the next higher power */
    }

    return result;
}
