/*
 * A294248  Number of partitions of 2n into two distinct squarefree parts.
 * https://oeis.org/A294248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n-1} mu(i)^2 * mu(2n-i)^2
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It counts pairs of distinct square‑free parts summing to 2n. */

#include <stdint.h>
#include <stdlib.h>

static uint8_t *sqf = NULL;     /* square‑free indicator, 1 = square‑free, 0 = not */
static size_t sqf_size = 0;    /* number of elements allocated (≤ current limit + 1) */

/* Ensure the square‑free table covers 0 .. limit (inclusive). */
static void ensure_sqf(size_t limit)
{
    if (limit < sqf_size)
        return;               /* already large enough */

    size_t new_size = limit + 1;
    uint8_t *tmp = (uint8_t *)realloc(sqf, new_size);
    if (!tmp)
        abort();              /* memory error, terminate */
    sqf = tmp;

    /* initialise all entries to square‑free */
    for (size_t i = 0; i <= limit; ++i)
        sqf[i] = 1;

    /* mark multiples of squares as non–square‑free */
    for (size_t p = 2; p * p <= limit; ++p)
    {
        size_t pp = p * p;
        for (size_t j = pp; j <= limit; j += pp)
            sqf[j] = 0;
    }

    sqf_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* We need to examine numbers up to 2n, so build the table accordingly */
    ensure_sqf((size_t)(2 * n));

    seq_t cnt = 0;
    for (idx_t i = 1; i < n; ++i)
    {
        if (sqf[(size_t)i] && sqf[(size_t)(2 * n - i)])
            ++cnt;
    }
    return cnt;
}
