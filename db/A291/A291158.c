/*
 * A291158  Sequence of black numbers on the single zero (European) roulette
 *          wheel read counterclockwise.
 * https://oeis.org/A291158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = black number at position n in counter‑clockwise order
 *            around the European roulette wheel
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Wheel numbers in clockwise order starting from 0. */
static const idx_t wheel[37] = {
    0, 32, 15, 19, 4, 21, 2, 25, 17, 34, 6, 27, 13,
    36, 11, 30, 8, 23, 10, 5, 24, 16, 33, 1, 20, 14,
    31, 9, 22, 18, 29, 7, 28, 12, 35, 3, 26
};

/* Bit mask for the 18 red numbers (index 0 is green). */
static const unsigned long long RED_MASK =
    (1ULL << 1) | (1ULL << 3) | (1ULL << 5) | (1ULL << 7) | (1ULL << 9) |
    (1ULL << 12) | (1ULL << 14) | (1ULL << 16) | (1ULL << 18) |
    (1ULL << 19) | (1ULL << 21) | (1ULL << 23) | (1ULL << 25) |
    (1ULL << 27) | (1ULL << 30) | (1ULL << 32) | (1ULL << 34) |
    (1ULL << 36);

/* true if v is a red number. */
static inline int is_red(idx_t v)
{
    return ((RED_MASK >> v) & 1ULL) != 0;
}

/* Cache of the 18 black numbers in counter‑clockwise order. */
static seq_t black_cycle[18];
static idx_t black_cycle_ready = 0;

/* External linkage. */
seq_t a(idx_t n)
{
    /* lazy initialization */
    if (!black_cycle_ready)
    {
        idx_t cnt = 0;
        /* iterate wheel in reverse (counter‑clockwise) skipping 0 */
        for (idx_t i = 36; i > 0; --i)
        {
            idx_t v = wheel[i];
            if (!is_red(v))
            {
                black_cycle[cnt++] = (seq_t)v;
                if (cnt == 18)
                    break;
            }
        }
        black_cycle_ready = 1;
    }
    idx_t idx = (n - 1) % 18;
    return black_cycle[idx];
}
