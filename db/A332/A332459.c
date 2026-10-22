/*
 * A332459  Odd part of 1+sigma(n).
 * https://oeis.org/A332459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd_part(1 + sigma(n))
 * Range:     u8 n <= 107, u16 n <= 7281, u32 n <= 7281, u64 n <= 7281,
 *            u128 n <= 7281, big any n
 * Helpers:   odd_part, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/odd_part.h"

seq_t a(idx_t n) {
    seq_t s = sigma_of(n);
    s = s + (seq_t)1;
    return odd_part(s);
}
