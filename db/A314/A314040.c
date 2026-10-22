/*
 * A314040  Coordination sequence Gal.4.133.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + d(1) + Σ_{k=2..n} d(k) where d(1)=4 and for k≥2: if
 *            k%7==0 or k%7==1 → d(k)=5 if k%7==2 or k%7==4 or k%7==6 → d(k)=6
 *            if k%7==3 or k%7==5 → d(k)=4
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t prefix[8] = {0,6,10,16,20,26,31,36}; /* prefix sums of one full cycle (k=2..8) */

    if (n == 0) return (seq_t)1;

    /* d(1) contributes 4 */
    seq_t sum = (seq_t)4;

    if (n > 1) {
        idx_t m = n - 1;           /* number of terms from k=2 to n inclusive */
        idx_t cycles = m / 7;
        idx_t rem    = m % 7;

        sum += (seq_t)cycles * (seq_t)36 + prefix[rem];
    }

    return (seq_t)1 + sum;
}
