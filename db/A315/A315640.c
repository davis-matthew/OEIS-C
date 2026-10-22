/*
 * A315640  Coordination sequence Gal.6.556.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A315640
   Coordination sequence Gal.6.556.5
   Formula:  a(n) = a(n-7) + B[n%7]  for n >= 7,
   where B[0]=38, B[1]=38, B[2]=37, B[3]=37,
   B[4]=37, B[5]=37, B[6]=38.
   Initial values: a(0)=1, a(1)=6, a(2)=12, a(3)=16,
   a(4)=21, a(5)=25, a(6)=32, a(7)=38, a(8)=44,
   a(9)=49, a(10)=53, a(11)=58, a(12)=62, a(13)=70.
*/

#include <stdint.h>

static const seq_t init_values[7] = {1, 6, 12, 16, 21, 25, 32};
static const seq_t base_values[7] = {38, 44, 49, 53, 58, 62, 70};
static const seq_t step_values[7] = {38, 38, 37, 37, 37, 37, 38};

seq_t a(idx_t n)
{
    if (n < 7) {
        return init_values[n];
    }
    idx_t k = n / 7;            /* k >= 1 */
    idx_t r = n % 7;            /* residue 0..6 */
    /* a(n) = base_values[r] + (k-1)*step_values[r] */
    seq_t result = base_values[r] + (seq_t)(k - 1) * step_values[r];
    return result;
}
