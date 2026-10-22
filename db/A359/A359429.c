/*
 * A359429  a(n) = 1 if n is cubefree, but not squarefree, otherwise 0.
 * https://oeis.org/A359429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9866, u16 n <= 9866, u32 n <= 9866, u64 n <= 9866,
 *            u128 n <= 9866, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A359429
   a(n) = 1 if n is cubefree but not squarefree, otherwise 0. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n is an unsigned integer type. */
    uint64_t m = (uint64_t)n;
    if (m <= 1) return (seq_t)0;   /* 1 is squarefree, so a(1)=0 */

    bool has_square = false;
    bool has_cube   = false;

    /* Handle factor 2 separately for efficiency. */
    if ((m & 1ULL) == 0ULL) {
        uint64_t c = 0;
        while ((m & 1ULL) == 0ULL) {
            m >>= 1;
            ++c;
            if (c > 2) { has_cube = true; break; }
        }
        if (!has_cube && c == 2) has_square = true;
    }

    /* Factor odd primes. */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t c = 0;
            while (m % p == 0) {
                m /= p;
                ++c;
                if (c > 2) { has_cube = true; break; }
            }
            if (has_cube) break;
            if (c == 2) has_square = true;
        }
    }

    if (has_cube)          return (seq_t)0;
    return has_square ? (seq_t)1 : (seq_t)0;
}
