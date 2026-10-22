/*
 * A347534  Number of partitions of n into at most 3 distinct squares.
 * https://oeis.org/A347534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9571, u16 n <= 9571, u32 n <= 9571, u64 n <= 9571,
 *            u128 n <= 9571, big any n
 * Helpers:   fsqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partition of n into at most 3 distinct squares */
/* Count is the number of subsets of distinct squares (size 1–3) whose sum is n.
   For n = 0 there is one (empty) partition. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/fsqrt.h"

seq_t a(idx_t n)
{
    /* The empty partition for n = 0 */
    if (n == 0) return (seq_t)1;

    seq_t count = 0;

    /* Largest square index that could appear */
    idx_t max_sq = fsqrt(n);

    for (idx_t i = 1; i <= max_sq; ++i) {
        idx_t a_sq = i * i;          /* first square */
        if (a_sq > n) break;
        if (a_sq == n) {             /* one‑square partition */
            count++;
            continue;                /* no larger parts possible */
        }

        for (idx_t j = i + 1; j <= max_sq; ++j) {
            idx_t b_sq = j * j;      /* second square */
            idx_t sum2 = a_sq + b_sq;
            if (sum2 > n) break;     /* further j would only increase */

            if (sum2 == n) {         /* two‑square partition */
                count++;
            }

            for (idx_t k = j + 1; k <= max_sq; ++k) {
                idx_t c_sq = k * k;  /* third square */
                idx_t sum3 = sum2 + c_sq;
                if (sum3 > n) break; /* further k would only increase */

                if (sum3 == n) {     /* three‑square partition */
                    count++;
                }
            }
        }
    }

    return count;
}
