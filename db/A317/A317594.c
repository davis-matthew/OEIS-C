/*
 * A317594  Lexicographically first sequence of different terms such that
 *          erasing the last three digits of a(n+1) and adding this new
 *          reshaped integer to a(n) gives back a(n+1).
 * https://oeis.org/A317594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4751, u32 n <= 9932, u64 n <= 9932,
 *            u128 n <= 9932, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(1) = 999
 *          for n > 1,
 *          let k = a(n‑1) / 999 (integer division).
 *          if a(n‑1) % 999 == 0 {
 *              if k == 1
 *                  a(n) = a(n‑1) + 1;
 *              else
 *                  a(n) = a(n‑1) + (k – 1);
 *          } else
 *              a(n) = a(n‑1) + k;
 *
 * (This implements the lexicographically first sequence of distinct terms
 *  for which a(n‑1) + floor(a(n)/1000) = a(n).)
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)999;

    seq_t prev = (seq_t)999;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t div = prev / (seq_t)999;          /* floor(prev/999)   */
        if (prev % (seq_t)999 == (seq_t)0) {     /* divisible by 999  */
            if (div == (seq_t)1) {
                prev = prev + (seq_t)1;         /* 999 → 1000       */
            } else {
                prev = prev + (div - (seq_t)1);
            }
        } else {
            prev = prev + div;
        }
    }
    return prev;
}
