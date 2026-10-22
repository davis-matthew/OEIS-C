/*
 * A340589  Number of partitions of n into 4 parts with at least one odd part.
 * https://oeis.org/A340589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into 4 positive parts with at
 *            least one odd part. a(n) = P4_exact(n) – [n even] *
 *            P4_exact(n/2), where P4_exact(m) = Σ_{k=1..⌊m/4⌋} ⌊((m‑4k)² +
 *            6*(m‑4k) + 12)/12⌋ This follows from the identity that all-even
 *            partitions of n correspond to partitions of n/2 into exa...
 * Range:     u8 n <= 32, u16 n <= 210, u32 n <= 8519, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   part_exact4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/part_exact4.h"

/* Public entry point */
seq_t a(idx_t n)
{
    seq_t total = part_exact4(n);

    if ((n & (idx_t)1) == 0)  /* n even */
    {
        seq_t sub = part_exact4(n / 2);
        if (total >= sub)
            total -= sub;
        else
            total = (seq_t)0;   /* safety; should not happen */
    }

    return total;
}
