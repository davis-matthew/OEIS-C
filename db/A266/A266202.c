/*
 * A266202  Weak Goodstein numbers: a(n) = g_n(n), where g_n(n) is the weak
 *          Goodstein function.
 * https://oeis.org/A266202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 65, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Weak Goodstein numbers: a(n) = g_n(n)
   g_0(n) = n
   For k = 1 .. n:
        g_k(n) = b_{k+1}( g_{k-1}(n) ) - 1
   Where b_b(m) converts the base‑b representation of m into
   base‑(b+1) using the same digits:
        if m = Σ d_i b^i   (0 ≤ d_i < b)
        then b_b(m) = Σ d_i (b+1)^i
   The following code implements this using only arithmetic on seq_t.
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    /* iterate over bases b = 2 .. n+1 */
    for (idx_t base = 2; base <= n + 1; ++base) {
        seq_t tmp = val;
        seq_t new_val = 0;
        seq_t pow = 1;                         /* (base+1)^position */
        if (tmp != 0) {
            while (tmp > 0) {
                seq_t digit = tmp % (seq_t)base;   /* digit in base 'base' */
                new_val += digit * pow;            /* add digit*(base+1)^pos */
                pow *= (seq_t)(base + 1);
                tmp /= (seq_t)base;
            }
        }
        /* subtract one (wraps correctly for seq_t types) */
        val = new_val - 1;
    }
    return val;
}
