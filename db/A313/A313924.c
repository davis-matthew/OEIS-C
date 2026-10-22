/*
 * A313924  Coordination sequence Gal.6.626.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-12)+66 for n>12 ; a(0)=1, a(1)=5, a(2)=10, a(3)=16,
 *            a(4)=22, a(5)=28, a(6)=33, a(7)=38, a(8)=44, a(9)=50, a(10)=56,
 *            a(11)=61, a(12)=66
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t base[13] = {
        (seq_t)1,  /* a(0) */
        (seq_t)5,  /* a(1) */
        (seq_t)10, /* a(2) */
        (seq_t)16, /* a(3) */
        (seq_t)22, /* a(4) */
        (seq_t)28, /* a(5) */
        (seq_t)33, /* a(6) */
        (seq_t)38, /* a(7) */
        (seq_t)44, /* a(8) */
        (seq_t)50, /* a(9) */
        (seq_t)56, /* a(10) */
        (seq_t)61, /* a(11) */
        (seq_t)66  /* a(12) */
    };

    if (n <= 12) {
        return base[(size_t)n];
    }

    idx_t k = n / 12;                 /* number of full 12-steps */
    idx_t r = n % 12;                 /* remainder 0..11 */

    if (r == 0) {                     /* multiple of 12, n > 12 */
        return base[12] + (seq_t)66 * ((seq_t)(k - 1));
    } else {                          /* general case */
        return base[(size_t)r] + (seq_t)66 * ((seq_t)k);
    }
}
