/*
 * A337442  Number of output sequences from the linear feedback shift register
 *          whose feedback polynomial coefficients (excluding the constant
 *          term) correspond to the binary representation of n.
 * https://oeis.org/A337442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A337442
 * Formula: a(n) = number of cycles of the linear feedback shift register
 * whose feedback polynomial (coefficients[1..d] are given by the binary
 * representation of n) acts on the vector space GF(2)^d.
 * The function counts all distinct cycles (including the zero cycle).
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Basic parity of a 64‑bit integer */
static inline unsigned int parity64(uint64_t x)
{
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (unsigned int)(x & 1);
}

/* Reverse the lowest `d` bits of `x` */
static inline uint64_t reverse_bits(uint64_t x, unsigned int d)
{
    uint64_t r = 0;
    for (unsigned int i = 0; i < d; ++i)
    {
        if ((x >> i) & 1ULL)
            r |= 1ULL << (d - 1 - i);
    }
    return r;
}

/* Compute the next state for the linear map -- see explanation */
static inline uint64_t next_state(uint64_t state,
                                 uint64_t tapmask,
                                 unsigned int d)
{
    return ((uint64_t)parity64(state & tapmask) << (d - 1)) | (state >> 1);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                /* Special case: zero polynomial only constant term */
        return 1;

    /* Determine degree d = floor_log2(n) + 1 */
    unsigned int d = 0;
    idx_t tmp = n;
    while (tmp) { tmp >>= 1; ++d; }

    /* Tap mask is the polynomial coefficients reversed (most recent bit first) */
    uint64_t tapmask = reverse_bits((uint64_t)n, d);

    /* Number of states = 2^d (d <= 63 for 64-bit n) */
    size_t size = 1ULL << d;   /* safe because d <= 63 in test data */

    /* Allocate visited array */
    uint8_t *visited = (uint8_t *)calloc(size, 1);
    if (!visited)
        return 0;             /* fallback on allocation failure */

    seq_t cycles = 0;
    for (size_t s = 0; s < size; ++s)
    {
        if (!visited[s])
        {
            size_t cur = s;
            while (!visited[cur])
            {
                visited[cur] = 1;
                cur = next_state(cur, tapmask, d);
            }
            if (cur == s)
                ++cycles;       /* new cycle detected */
        }
    }

    free(visited);
    return cycles;
}
