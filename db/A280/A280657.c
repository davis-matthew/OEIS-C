/*
 * A280657  Numbers beginning with their digital root in decimal
 *          representation.
 * https://oeis.org/A280657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 7320, u32 n <= 8624, u64 n <= 8624,
 *            u128 n <= 8624, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A280657:
   integers whose decimal representation begins with their digital root.
   The function memoizes results so any n ≥ 1 may be requested in any order. */
#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic array of sequence values */
static size_t cap   = 0;      /* current capacity of cache  */
static size_t len   = 0;      /* number of values computed so far   */

seq_t a(idx_t n)
{
    if (n == 0)                /* offset starts at 1 – not used but safe */
        return 0;

    /* generate terms up to the requested index */
    if (len < n) {
        seq_t cur = (len == 0 ? 0 : cache[len - 1] + 1);

        while (len < n) {
            /* digital root: 0 => 0, else 1 + ((x-1) % 9) */
            seq_t dr = (cur == 0 ? 0 : (1 + ((cur - 1) % 9)));

            /* find highest power of 10 <= cur for leading digit extraction */
            seq_t pow = 1;
            while (cur / pow >= 10) pow *= 10;
            seq_t lead = cur / pow;

            if (lead == dr) {               /* store term if it matches */
                if (len == cap) {           /* grow cache if full */
                    size_t newcap = (cap == 0 ? 16 : cap * 2);
                    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    cache = tmp;
                    cap   = newcap;
                }
                cache[len++] = cur;
            }
            ++cur;                          /* next integer to test */
        }
    }
    return cache[n - 1];
}
