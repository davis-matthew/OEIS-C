/*
 * A395635  a(n) = number of triples (x, y, z) such that 2*x^2 + y*z = n,
 *          where x, y, z are positive integers with gcd(x,y*z)>1.
 * https://oeis.org/A395635
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive integer triples (x,y,z) with * 2*x^2 +
 *            y*z = n and gcd(x, y*z) > 1 * The function returns the count as
 *            a value of type seq_t
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd, tau_from_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/tau_from_idx.h"

seq_t a(idx_t n)
{
    if (n < 10) return (seq_t)0;  /* behaviour for n < 10 is irrelevant */

    seq_t result = (seq_t)0;

    for (idx_t x = 1;; ++x) {
        idx_t twox2 = 2 * x * x;
        if (twox2 >= n) break;

        idx_t r = n - twox2;      /* r = y*z, must be > 0 */
        if (r == 0) continue;

        seq_t d = gcd((seq_t)x, (seq_t)r);
        if (d > (seq_t)1) {
            result += tau_from_idx(r);
        }
    }
    return result;
}
