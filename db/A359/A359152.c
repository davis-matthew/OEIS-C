/*
 * A359152  a(n) = 1 if n is a number of the form 4u+3 with an odd number of
 *          prime factors (counted with multiplicity), otherwise 0.
 * https://oeis.org/A359152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n ≡ 3 (mod 4) and Ω(n) is odd; otherwise 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 144 terms through n = 9620 (2026-11-01)
 * Helpers:   bigomega_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bigomega_seq.h"

seq_t a(idx_t n)
{
    if ((n & 3u) != 3u)
        return 0;
    seq_t omega = bigomega_seq((seq_t)n);
    if ((omega & 1u) != 0)
        return 1;
    return 0;
}
