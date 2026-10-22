/*
 * A385828  a(n) = Sum_{k=0..n} (binomial(n, k) mod 11).
 * https://oeis.org/A385828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (binomial(n, k) mod 11)
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External integer types defined by the build system */
typedef idx_t idx_t;   /* unsigned, e.g. uint64_t */
typedef seq_t seq_t;   /* unsigned of variable width, or arbitrary‑precision */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Allocate a byte buffer large enough for the largest required row.
       The buffer is reused on later calls. */
    static uint8_t *row = NULL;
    static size_t cap  = 0;

    /* Expand the buffer if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        if (row == NULL)
            row = (uint8_t *)calloc(new_cap, sizeof(uint8_t));
        else
            row = (uint8_t *)realloc(row, new_cap * sizeof(uint8_t));
        cap = new_cap;
    }

    /* Initialise the first row (n = 0) */
    row[0] = 1;

    /* Build rows up to n using Pascal's rule in place (right‑to‑left update) */
    for (idx_t i = 0; i < n; ++i) {
        /* New last element is always 1 */
        row[i + 1] = 1;
        /* Update interior from right to left to avoid overwriting needed values */
        for (idx_t k = i; k != 0; --k) {
            row[k] = (uint8_t)((row[k - 1] + row[k]) % 11);
        }
    }

    /* Sum the elements of the final row */
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i)
        sum += (seq_t)row[i];

    return sum;
}
