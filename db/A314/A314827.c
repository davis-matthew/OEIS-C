/*
 * A314827  Coordination sequence Gal.5.53.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=1+4*n+count5(n), * where count5(n) is the
 *            number of indices i∈[3,n] with (i-3)%10∈{0,5,7,8}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* only for clarity; seq_t and idx_t are defined elsewhere */

seq_t a(idx_t n)
{
    /* small n handled explicitly */
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 9;

    /* For n ≥ 3, use the pattern of increments */
    idx_t m = n - 3;                         /* shift so that m=0 corresponds to i=3 */
    idx_t full_cycles = (m + 1) / 10;        /* number of complete 10‑step cycles */
    seq_t count5 = (seq_t)full_cycles * 4;   /* 4 increments of 5 per full cycle */

    idx_t rem = (m + 1) % 10;                /* remainder steps in the last partial cycle */
    static const unsigned char inc5[10] = {
        1,0,0,0,0,1,0,1,1,0            /* 1 indicates (i-3)%10∈{0,5,7,8} */
    };
    for (idx_t t = 0; t < rem; ++t) {
        if (inc5[t]) ++count5;
    }

    /* a(n) = 1 + 4*n + count5(n) */
    return 1 + (seq_t)4 * n + count5;
}
