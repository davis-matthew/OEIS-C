/*
 * A283106  Number of distinct envelope areas of the polyominoes of order n.
 * https://oeis.org/A283106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ w*h : w ≥ 1, h ≥ 1, w*h ≥ n, w+h-1 ≤ n }|
 * Range:     u8 n <= 45, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts the distinct envelope areas of polyominoes with n cells. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 in the OEIS entry. */
    if (n == 0)
        return 1;

    size_t N = (size_t)n;
    size_t max_area = N * N;               /* maximum possible envelope area */
    size_t words = (max_area + 63) >> 6;    /* ceil(max_area/64) bits */

    uint64_t *bits = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!bits)
        return 0;                         /* allocation failure – safe fallback */

    for (size_t w = 1; w <= N; ++w)
    {
        size_t L = (N + w - 1) / w;        /* ceil(N / w)  → minimal h */
        size_t U = N + 1 - w;              /* maximal h from w+h-1 ≤ N */
        if (L > U)
            continue;

        for (size_t h = L; h <= U; ++h)
        {
            size_t area = w * h;           /* area of the envelope */
            bits[area >> 6] |= 1ULL << (area & 63);
        }
    }

    seq_t cnt = 0;
    for (size_t i = 0; i < words; ++i)
        cnt += (seq_t)__builtin_popcountll(bits[i]);

    free(bits);
    return cnt;
}
