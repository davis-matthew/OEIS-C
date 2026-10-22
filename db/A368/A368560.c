/*
 * A368560  a(1) = 78557 (the first Sierpinski number); thereafter a(n+1) =
 *          Od(3*5*7*13*19*37*73 - a(n)), where Od(m) is the odd part of m.
 * https://oeis.org/A368560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=78557; for n>1, a(n)=odd_part(P - a(n-1)), 
   with P = 3*5*7*13*19*37*73 = 70050435.
   The sequence is periodic with period 14. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/odd_part.h"

static inline seq_t odd_part_uint64(uint64_t x)
{
    while ((x & 1ULL) == 0)
        x >>= 1;
    return (seq_t)x;
}

seq_t a(idx_t n)
{
    static seq_t cached[14];
    static int initialized = 0;
    if (!initialized) {
        const uint64_t P = 70050435ULL;
        cached[0] = 78557;
        for (int i = 1; i < 14; ++i) {
            uint64_t prev = (uint64_t)cached[i - 1];
            uint64_t val = P - prev;
            cached[i] = odd_part_uint64(val);
        }
        initialized = 1;
    }
    size_t idx = (size_t)((n - 1) % 14);
    return cached[idx];
}
