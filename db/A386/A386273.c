/*
 * A386273  Expansion of 1/(1 - 49*x)^(4/7).
 * https://oeis.org/A386273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386273: Expansion of 1/(1 - 49*x)^(4/7)
   Recurrence: a(0) = 1, for n ≥ 1
                 a(n) = ((49*n - 21)/n) * a(n-1)
   Alternative product form:
      a(n) = 7^n / n! * ∏_{k=0}^{n-1} (7*k + 4)   (n(0) = 1) */
   
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }

    seq_t result = 1;

#ifdef __SIZEOF_INT128__
    /* If seq_t fits in at most 64 bits, use a 128‑bit intermediate
       to avoid overflow during the multiplication step.  */
    if (sizeof(seq_t) <= 8) {
        for (idx_t i = 1; i <= n; ++i) {
            unsigned __int128 num = (unsigned __int128)49 * i - 21;   /* 49*i - 21 */
            unsigned __int128 prod = (unsigned __int128)result * num;
            result = (seq_t)(prod / i);
        }
        return result;
    }
#endif

    /* Generic fallback that works for arbitrary‑precision seq_t.
       It repeatedly cancels factors of the denominator before
       performing the multiplication, so no intermediate overflow
       occurs in arbitrary‑precision arithmetic.   */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t num = 7 * (7 * (seq_t)i - 3);      /* 49*i - 21 */
        seq_t g1 = gcd(num, (seq_t)i);
        num /= g1;
        seq_t i1 = i / g1;

        seq_t g2 = gcd(result, i1);
        result /= g2;
        i1 /= g2;   /* Now i1 should be 1 or divide result exactly */

        result = result * num;   /* Safe – arbitrary precision */
        if (i1 != 0) {
            result = result / i1; /* Integer division */
        }
    }
    return result;
}
