/*
 * A315471  Coordination sequence Gal.3.52.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            a(n-6) for n > 6, with initial values a(0)=1, a(1)=6, a(2)=11,
 *            a(3)=16, a(4)=21, a(5)=26, a(6)=32
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the n-th term of OEIS sequence A315471 (Coordination sequence Gal.3.52.3). */
seq_t a(idx_t n)
{
    /* Return the fixed initial values for n ≤ 6. */
    if (n <= 6) {
        switch (n) {
            case 0: return 1;
            case 1: return 6;
            case 2: return 11;
            case 3: return 16;
            case 4: return 21;
            case 5: return 26;
            case 6: return 32;
            default: return 0; /* unreachable */
        }
    }

    /* Sliding window of the last six terms:
       t0 … t5 hold a(n-5), … , a(n).  Initially they contain a(1) … a(6). */
    seq_t t0 = 6;   /* a(1) */
    seq_t t1 = 11;  /* a(2) */
    seq_t t2 = 16;  /* a(3) */
    seq_t t3 = 21;  /* a(4) */
    seq_t t4 = 26;  /* a(5) */
    seq_t t5 = 32;  /* a(6) */

    seq_t next = 0; /* placeholder for a(n) in the loop */

    for (idx_t i = 7; i <= n; ++i) {
        /* Apply the recurrence: a(i) = 2*a(i-1) - 2*a(i-2) + 2*a(i-3)
           - 2*a(i-4) + 2*a(i-5) - a(i-6).  */
        next = 2*t5 - 2*t4 + 2*t3 - 2*t2 + 2*t1 - t0;

        /* Advance the sliding window. */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = t5;
        t5 = next;
    }

    return next;
}
