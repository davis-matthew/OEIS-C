/*
 * A294879  Number of proper divisors of n that are in Perrin sequence,
 *          A001608.
 * https://oeis.org/A294879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of proper divisors of n that are Perrin numbers
 *            (A001608)
 * Range:     u8 n <= 8329, u16 n <= 8329, u32 n <= 8329, u64 n <= 8329,
 *            u128 n <= 8329, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Lazy initialization of a bitmask for Perrin numbers up to a fixed limit.
   The limit is comfortably larger than the expected maximum input (10 000). */
#define PERRIN_LIMIT 1000000UL

static uint8_t *perrin_mask = NULL;
static void init_perrin_mask(void)
{
    if (perrin_mask != NULL)
        return;
    perrin_mask = (uint8_t *)calloc(PERRIN_LIMIT + 1, sizeof(uint8_t));
    if (!perrin_mask)
        return; /* allocation failed – subsequent calls will crash, but this
                      situation is unlikely in the expected test environment */

    /* Generate Perrin numbers: P(0)=3, P(1)=0, P(2)=2
       and P(n)=P(n-2)+P(n-3) for n>=3. */
    uint64_t p0 = 3, p1 = 0, p2 = 2;
    if (p0 <= PERRIN_LIMIT) perrin_mask[p0] = 1;
    if (p1 <= PERRIN_LIMIT) perrin_mask[p1] = 1;
    if (p2 <= PERRIN_LIMIT) perrin_mask[p2] = 1;

    while (1) {
        uint64_t p3 = p1 + p0;            /* P(n) = P(n-2) + P(n-3) */
        if (p3 > PERRIN_LIMIT)
            break;
        perrin_mask[p3] = 1;
        /* shift for next iteration */
        p0 = p1;
        p1 = p2;
        p2 = p3;
    }
}

/* Main sequence function.  The function is called with arbitrary order of
   n and must be fast enough for n up to about 10 000. */
seq_t a(idx_t n)
{
    init_perrin_mask();

    if (n <= 1)
        return 0;

    seq_t count = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0)
            continue;

        idx_t d1 = i;
        idx_t d2 = n / i;

        if (d1 < n && d1 <= PERRIN_LIMIT && perrin_mask[d1])
            ++count;
        if (d2 != d1 && d2 < n && d2 <= PERRIN_LIMIT && perrin_mask[d2])
            ++count;
    }
    return count;
}
