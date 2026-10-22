/*
 * A335208  Number of Heronian triangles with perimeter A335101(n) whose
 *          middle side length is a square.
 * https://oeis.org/A335208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   grow_seq_array, is_square_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes OEIS sequence A335208: Number of Heronian triangles
 * with perimeter A335101(n) whose middle side length is a square.
 *
 * The function enumerates successive perimeters, counts all
 * Heronian triangles with a square middle side for each,
 * stores the counts and answers queries in any order.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/grow_seq_array.h"
#include "../utils/is_square_uint128.h"

static seq_t *g_counts = NULL;
static size_t g_counts_len = 0;
static size_t g_counts_cap = 0;
static idx_t g_next_p = 12; /* start from the first known perimeter */

seq_t a(idx_t n)
{
    /* Ensure we have computed up to the requested index */
    while (g_counts_len < n)
    {
        idx_t p = g_next_p++;
        if ((p & 1) != 0) continue;              /* perimeter must be even */
        uint64_t p_half = (uint64_t)(p >> 1);     /* s = p/2 */
        uint64_t max_b = (p - 1) / 2;             /* b <= floor((p-1)/2) */
        seq_t cnt = 0;

        /* iterate over all square values of b */
        for (uint64_t k = 1; ; ++k)
        {
            uint64_t b = k * k;
            if (b > max_b) break;

            uint64_t max_a = p - 2 * b;
            if (max_a < 1) continue;

            int64_t a_low = (int64_t)(p_half - b + 1);
            if (a_low < 1) a_low = 1;
            uint64_t a_up = (b < max_a ? b : max_a);
            if (a_low > (int64_t)a_up) continue;

            for (uint64_t a = a_low; a <= a_up; ++a)
            {
                uint64_t c = p - a - b;
                if (c < b) continue;          /* maintain a <= b <= c */

                /* Compute squared area using Heron's formula */
                unsigned __int128 t =
                    (unsigned __int128)p_half *
                    (p_half - a) *
                    (p_half - b) *
                    (p_half - c);

                if (t > 0 && is_square_uint128(t))
                    cnt++;
            }
        }

        if (cnt > 0)
        {
            if (g_counts_len == g_counts_cap)
                grow_seq_array(&g_counts, &g_counts_cap, g_counts_len + 1);
            g_counts[g_counts_len++] = cnt;
        }
    }

    return g_counts[n - 1];
}
