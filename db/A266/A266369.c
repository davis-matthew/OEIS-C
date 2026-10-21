/*
 * A266369  Four-digit primes such that simultaneously swapping the first two
 *          digits and swapping the last two digits produces another prime.
 * https://oeis.org/A266369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 201, u32 n <= 201, u64 n <= 201, u128 n <= 201,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: naive search of 4‑digit primes p such that swapping
   the first two digits and the last two digits (p = abcd → bcda)
   yields another prime (leading zero after the swap is ignored). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_small.h"

static seq_t *qualifying = NULL;
static size_t qcnt = 0;
static bool init_done = false;

static void init(void)
{
    const size_t MAX = 2000;
    qualifying = (seq_t *)malloc(MAX * sizeof(seq_t));
    if (!qualifying)
        return;   /* allocation failed – missing terms will be 0 */

    for (uint64_t p = 1000; p <= 9999; ++p) {
        if (!is_prime_small(p))
            continue;

        uint64_t a = p / 1000;          /* thousands digit   */
        uint64_t b = (p / 100) % 10;    /* hundreds digit    */
        uint64_t c = (p / 10) % 10;     /* tens digit        */
        uint64_t d = p % 10;            /* units digit       */

        /* swap first two and last two digits simultaneously:
           new order: b a d c   (leading zero after swap is ignored) */
        uint64_t swapped = b * 1000 + a * 100 + d * 10 + c;

        if (is_prime_small(swapped)) {
            if (qcnt < MAX)
                qualifying[qcnt++] = (seq_t)p;
        }
    }
}

seq_t a(idx_t n)
{
    if (!init_done) {
        init();
        init_done = true;
    }

    if (n == 0)
        return (seq_t)0;                /* offset starts at 1 */

    size_t idx = (size_t)(n - 1);
    if (idx < qcnt)
        return qualifying[idx];
    return (seq_t)0;                  /* term beyond available list */
}
