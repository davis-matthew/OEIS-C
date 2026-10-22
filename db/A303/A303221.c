/*
 * A303221  Total area of all rectangles with dimensions p and p + q such that
 *          p and q are both squarefree, n = p + q and p <= q.
 * https://oeis.org/A303221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Σ_{i=1}^{⌊n/2⌋} i * mu(i)^2 * mu(n-i)^2
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts the total area of all square‑free rectangles of
   dimensions p × q with p + q = n and p ≤ q, using the Möbius
   squares indicator. */

#include <stdint.h>
#include <stdlib.h>

/* Static square‑free indicator array and its size */
static idx_t sf_limit = 0;
static uint8_t *sf_array = NULL;

/* Ensure the square‑free array is computed up to 'limit' */
static void ensure_squarefree(idx_t limit)
{
    if (limit <= sf_limit)           /* already sufficient */
        return;

    /* Allocate new array */
    size_t size = (size_t)(limit + 1);
    uint8_t *new_sf = (uint8_t *)calloc(size, 1);
    if (!new_sf)
        return;                     /* allocation failure – leave old array */

    /* Mark every number as square‑free initially */
    for (idx_t i = 1; i <= limit; ++i)
        new_sf[(size_t)i] = 1;

    /* Eliminate numbers divisible by any square ≥ 4 */
    for (idx_t j = 2; (idx_t)j * (idx_t)j <= limit; ++j) {
        idx_t sq = (idx_t)j * (idx_t)j;
        for (idx_t k = sq; k <= limit; k += sq)
            new_sf[(size_t)k] = 0;
    }

    /* Replace the old array */
    free(sf_array);
    sf_array = new_sf;
    sf_limit = limit;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                   /* undefined for n < 1, return 0 */

    /* Prepare square‑free table up to n */
    ensure_squarefree(n);

    seq_t sum = 0;
    for (idx_t i = 1; i <= n / 2; ++i) {
        if (sf_array[(size_t)i] && sf_array[(size_t)(n - i)]) {
            sum += (seq_t)i;
        }
    }
    return (seq_t)n * sum;
}
