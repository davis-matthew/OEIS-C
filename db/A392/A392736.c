/*
 * A392736  A 4-automatic binary sequence with irreducible nested recurrence.
 * https://oeis.org/A392736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1,
   a(4n)   = a(n),
   a(4n+1) = 1 - a(n),
   a(4n+2) = a(2n+1-a(n)),
   a(4n+3) = 1 - a(2n+1-a(n)) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)                /* base cases a(0)=0, a(1)=1 */
        return (seq_t)n;

    idx_t q = n >> 2;                    /* floor(n/4) */
    unsigned r = (unsigned)(n & 3U);     /* n mod 4 */

    switch (r) {
    case 0:
        return a(q);
    case 1:
        return 1 - a(q);
    case 2: {
        seq_t t = a(q);
        idx_t idx = (q << 1) + 1 - (idx_t)t;   /* 2*q + 1 - a(q) */
        return a(idx);
    }
    case 3: {
        seq_t t = a(q);
        idx_t idx = (q << 1) + 1 - (idx_t)t;   /* 2*q + 1 - a(q) */
        return 1 - a(idx);
    }
    default:
        return 0; /* unreachable */
    }
}
