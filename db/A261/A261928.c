/*
 * A261928  a(n) is the number of different pairs (x*y,x+y) mod n.
 * https://oeis.org/A261928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of distinct pairs (x*y mod n, x + y mod n)
 * Range:     u8 n <= 22, u16 n <= 366, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Allocate a bitset of size n*n bits. */
    size_t total = (size_t)n * n;           /* total number of pairs (x,y) */
    size_t words = (total + 63) >> 6;       /* number of 64‑bit words needed */
    uint64_t *bits = (uint64_t *)malloc(words * sizeof(uint64_t));
    if (!bits) return 0;
    for (size_t i = 0; i < words; ++i) bits[i] = 0;

    /* Enumerate all (x,y) and set the corresponding bit. */
    for (idx_t x = 0; x < n; ++x) {
        idx_t s = x;        /* sum mod n for y = 0 */
        idx_t p = 0;        /* product mod n for y = 0 */

        for (idx_t y = 0; y < n; ++y) {
            size_t idx = (size_t)p * n + s;
            bits[idx >> 6] |= 1ULL << (idx & 63);

            /* Prepare values for the next y */
            if (++s == n) s = 0;
            if ((p += x) >= n) p -= n;
        }
    }

    /* Count set bits to obtain the number of distinct pairs. */
    seq_t count = 0;
    for (size_t i = 0; i < words; ++i)
        count += (seq_t)__builtin_popcountll(bits[i]);

    free(bits);
    return count;
}
