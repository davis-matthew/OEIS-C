/*
 * A286990  {0->010, 1->101}-transform of the Sturmian word A080764.
 * https://oeis.org/A286990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3‑bit morphism {0->010, 1->101} applied to the Sturmian
 *            word defined by a_k = floor((k+1)*α) − floor(k*α) with α = 1/√2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned 64‑bit integer. */
static uint64_t u64_isqrt(uint64_t x)
{
    uint64_t low  = 0;
    uint64_t high = (uint64_t)1 << 32;        /* (2^32)^2 = 2^64 fits in uint64 */
    while (low <= high)
    {
        uint64_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high; /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* Step 1: locate the corresponding symbol in the original Sturmian word. */
    uint64_t orig = (n - 1) / 3 + 1;   /* 1‑based index in A080764   */
    uint64_t off  = (n - 1) % 3;       /* position inside the 3‑bit block */

    /* Step 2: compute the value of the Sturmian word at position orig.
       The word is defined by α = 1/√2, so
       a_k = ⌊(k+1)*α⌋ – ⌊k*α⌋.
       Use the identity ⌊k*α⌋ = ⌊⌊k*√2⌋ / 2⌋, where ⌊k*√2⌋ = ⌊√(2k²)⌋. */
    uint64_t val1 = (uint64_t)((unsigned __int128)2 * (orig + 1) * (orig + 1));
    uint64_t val0 = (uint64_t)((unsigned __int128)2 * orig * orig);

    uint64_t f1 = u64_isqrt(val1);
    uint64_t f0 = u64_isqrt(val0);

    uint64_t bit = (f1 / 2) - (f0 / 2);   /* 0 or 1 */

    /* Step 3: apply the morphism 0→010, 1→101. */
    uint64_t out;
    if (bit == 0) {                /* 010 */
        out = (off == 0) ? 0 :
              (off == 1) ? 1 :
              0;
    } else {                       /* 101 */
        out = (off == 0) ? 1 :
              (off == 1) ? 0 :
              1;
    }

    return (seq_t)out;
}
