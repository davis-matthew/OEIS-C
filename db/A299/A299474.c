/*
 * A299474  a(n) = 4*p(n), where p(n) is the number of partitions of n.
 * https://oeis.org/A299474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*A000041(n) (four times the partition number)
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 115, u64 n <= 393,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *part = NULL;   /* partition numbers p(0)...p(n) */
static idx_t  part_max = 0;  /* largest index for which p(i) is computed */

/* Extend the partition table to index n, computing p(k) for all
   k in (part_max, n] using Euler's pentagonal recurrence. */
static void ensure_part(idx_t n)
{
    /* Initial allocation / recompute up to n */
    if (part == NULL) {
        part = (seq_t *)calloc(n + 1, sizeof(seq_t));
        if (part == NULL) exit(1);
        part[0] = 1;            /* p(0) = 1 */
        if (n >= 1) {
            for (idx_t k = 1; k <= n; ++k) {
                seq_t pos = 0, neg = 0;
                int   sign = 1;   /* +1, -1, +1, ... */
                for (idx_t m = 1; ; ++m) {
                    idx_t g1 = m * (3 * m - 1) / 2;
                    if (g1 > k) break;
                    if (sign > 0) pos += part[k - g1];
                    else          neg += part[k - g1];

                    idx_t g2 = m * (3 * m + 1) / 2;
                    if (g2 <= k) {
                        if (sign > 0) pos += part[k - g2];
                        else          neg += part[k - g2];
                    }
                    sign = -sign;
                }
                part[k] = pos - neg;     /* pos >= neg */
            }
        }
        part_max = n;
        return;
    }

    /* Existing part table is valid up to part_max. Extend if needed. */
    if (n > part_max) {
        size_t old_size = part_max + 1;
        size_t new_size = n + 1;
        seq_t *new_part = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (new_part == NULL) exit(1);
        memcpy(new_part, part, old_size * sizeof(seq_t));
        free(part);
        part = new_part;

        for (idx_t k = part_max + 1; k <= n; ++k) {
            seq_t pos = 0, neg = 0;
            int   sign = 1;
            for (idx_t m = 1; ; ++m) {
                idx_t g1 = m * (3 * m - 1) / 2;
                if (g1 > k) break;
                if (sign > 0) pos += part[k - g1];
                else          neg += part[k - g1];

                idx_t g2 = m * (3 * m + 1) / 2;
                if (g2 <= k) {
                    if (sign > 0) pos += part[k - g2];
                    else          neg += part[k - g2];
                }
                sign = -sign;
            }
            part[k] = pos - neg;
        }
        part_max = n;
    }
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    ensure_part(n);
    return part[n] * 4;
}
