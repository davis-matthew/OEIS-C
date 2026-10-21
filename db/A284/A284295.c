/*
 * A284295  Numbers k such that product of digits of k is a power of 9.
 * https://oeis.org/A284295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k whose product of digits is a power of 9 (OEIS A284295) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_product.h"

/* Static buffer large enough for n up to about 10000 (the test limits). */
#define MAX_CACHE 10000

static seq_t seq_mem[MAX_CACHE];   /* stored terms */
static size_t seq_size = 0;        /* number of terms currently stored */
static seq_t seq_next = 1;         /* integer to test next */

/* Return the n‑th term (1‑based) of A284295. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1, guard against misuse */

    /* Grow the cache until it contains at least n elements. */
    while (seq_size < (size_t)n && seq_size < MAX_CACHE) {
        seq_t prod = digit_product(seq_next);  /* product of digits */

        bool ok = false;
        if (prod != 0) {
            if (prod == 1)          /* 9^0 = 1 is allowed */
                ok = true;
            else {
                seq_t t = prod;
                /* Strip all factors of 9. */
                while (t % 9 == 0 && t != 0)
                    t /= 9;
                if (t == 1)
                    ok = true;
            }
        }

        if (ok)
            seq_mem[seq_size++] = seq_next;

        seq_next += 1;
    }

    /* If n exceeds MAX_CACHE, the sequence cannot be computed exactly
       under the current implementation. */
    if (seq_size < (size_t)n)
        return 0;

    return seq_mem[(size_t)(n - 1)];
}
