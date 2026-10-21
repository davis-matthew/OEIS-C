/*
 * A259982  Decimal expansion of 1/2^20.
 * https://oeis.org/A259982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = floor(10^(n+1)/2^20) - floor(10^n/2^20)
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    const uint64_t DEN = 1ULL << 20;   /* 2^20 = 1048576 */
    if (n >= 20) return 0;            /* digits after the 20th are 0 */
    uint64_t r = 1;                   /* remainder before extraction */
    uint64_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;                      /* shift decimal point right */
        digit = r / DEN;              /* digit at position i */
        r %= DEN;                     /* new remainder for next step */
    }
    return (seq_t)digit;
}
