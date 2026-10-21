/*
 * A284792  {1001->1}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A284792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth term of the word obtained from the Thue-Morse word by
 *            * replacing each non‑overlapping occurrence of the pattern 1001
 *            with a single * 1 (scanning left to right). That word is the
 *            A284792 sequence
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t thue_bit(idx_t i)
{
#if defined(__GNUC__) || defined(__clang__)
    /* GCC/Clang provide a fast builtin parity of a 64‑bit integer. */
    return (seq_t)__builtin_parityll(i);
#else
    /* Portable fallback: compute parity of the 64‑bit integer i. */
    idx_t x = i;
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (seq_t)(x & 1);
#endif
}

seq_t a(idx_t n)
{
    idx_t   out_cnt = 0;   /* how many transformed bits we have produced */
    idx_t   pos     = 0;   /* current index in the original Thue–Morse word */

    while (out_cnt < n)
    {
        /* Obtain the next four bits of the original Thue–Morse word. */
        seq_t b0 = thue_bit(pos);
        seq_t b1 = thue_bit(pos + 1);
        seq_t b2 = thue_bit(pos + 2);
        seq_t b3 = thue_bit(pos + 3);

        if (b0 == 1 && b1 == 0 && b2 == 0 && b3 == 1)
        {
            /* The pattern 1001 is found – replace it by a single 1. */
            ++out_cnt;
            if (out_cnt == n) return 1;
            pos += 4;   /* skip the four original bits of the pattern */
        }
        else
        {
            /* No pattern – keep the current bit of the Thue–Morse word. */
            ++out_cnt;
            if (out_cnt == n) return b0;
            ++pos;
        }
    }
    /* The loop always returns; this point is unreachable. */
    return 0;               /* silence compiler warnings */
}
