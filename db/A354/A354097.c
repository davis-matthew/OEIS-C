/*
 * A354097  a(n) = 1 if n-phi(n) is a multiple of 4, otherwise 0.
 * https://oeis.org/A354097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff n‑phi(n) is a multiple of 4, else 0
 * Range:     u8 n <= 515, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 141 terms through n = 9934 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t phi_n = totient_seq((seq_t)n);
    seq_t diff = (seq_t)n - phi_n;
    return (diff % 4 == 0) ? 1 : 0;
}
