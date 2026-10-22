/*
 * A354384  Difference sequence of A356133.
 * https://oeis.org/A354384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A007413(n) + 1 A007413(n) is obtained by the following
 *            rules: - Let tz be the number of trailing zeros in the binary
 *            representation of n. - If tz is odd, then A007413(n) = 2. - If
 *            tz is even: • Let r = n >> tz (the odd part of n). • Let pc =
 *            popcount(r). • If pc is odd, A007413(n) = 1. • I...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static unsigned int popcount_uint64(uint64_t x) {
    unsigned int c = 0;
    while (x) {
        c += (unsigned)(x & 1);
        x >>= 1;
    }
    return c;
}

/* External linkage as required */
seq_t a(idx_t n) {
    /* n is at least 1 */
    unsigned int tz = 0;
    uint64_t temp = (uint64_t)n;
    /* count trailing zeros */
    while ((temp & 1ULL) == 0 && temp != 0ULL) {
        ++tz;
        temp >>= 1;
    }

    if (tz & 1U) {              /* odd number of trailing zeros */
        return (seq_t)3;        /* A007413 = 2 → a = 3 */
    } else {
        uint64_t r = (uint64_t)n >> tz;
        unsigned int pc = popcount_uint64(r);
        seq_t aval = (pc & 1U) ? (seq_t)1 : (seq_t)3; /* 1 if odd, 3 if even */
        return aval + 1;        /* a = A007413 + 1 */
    }
}
