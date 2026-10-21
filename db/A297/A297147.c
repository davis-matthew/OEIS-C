/*
 * A297147  Numbers having a down-first zigzag pattern in base 10; see
 *          Comments.
 * https://oeis.org/A297147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   Down‑first zigzag in base 10. Computed by brute‑force search
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   has_down_first_zigzag
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_down_first_zigzag.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t num = 10;                  /* start from the smallest two‑digit number */

    while (1)
    {
        if (has_down_first_zigzag(num))
        {
            ++count;
            if (count == n)
                return num;
        }
        ++num;
    }
}
