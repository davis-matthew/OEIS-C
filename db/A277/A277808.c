/*
 * A277808  a(n) = number of iterations of map k -> A003188(A006068(k)/2) that
 *          are required (when starting from k = n) until k is an odious
 *          number.
 * https://oeis.org/A277808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of times that floor(n/2) must be applied until the
 *            resulting number is odious (i.e. has an odd number of 1‑bits in
 *            its binary representation)
 * Range:     u8 n <= 9617, u16 n <= 9617, u32 n <= 9617, u64 n <= 9617,
 *            u128 n <= 9617, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline unsigned int popcnt_uint64(uint64_t x)
{
    unsigned int c = 0;
    while (x) {
        c += (unsigned int)(x & 1ULL);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0;
    while (true) {
        /* check odious (odd number of 1‑bits) */
        if ((popcnt_uint64(n) & 1U) != 0)
            return (seq_t)cnt;
        n >>= 1;   /* floor(n/2) */
        ++cnt;
    }
}
