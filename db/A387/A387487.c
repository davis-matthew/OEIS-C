/*
 * A387487  a(n) is the length of the longest prefix of S matching the
 *          substring of S starting at n where S = 123456789101112... is the
 *          Champernowne word (A033307).
 * https://oeis.org/A387487
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of longest common prefix of Champernowne word and
 *            the same word shifted to start at position n (1‑based)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Powers of ten up to 10⁻¹⁸, enough for 64‑bit idx_t indices. */
static const idx_t POW10[19] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL, 1000000000000000000ULL
};

/* Return the decimal digit (0‑9) at 1‑based position pos in Champernowne
   word S = 123456789101112… */
static uint8_t cham_digit(idx_t pos)
{
    idx_t x = pos;
    idx_t d = 1;
    for (;; ++d) {                     /* find block of d‑digit numbers */
        idx_t count = 9ULL * POW10[d-1];
        idx_t len   = count * d;
        if (x <= len)
            break;
        x -= len;
    }
    idx_t offset      = (x - 1) / d;          /* zero‑based index within block */
    idx_t number      = POW10[d-1] + offset;  /* the actual number */
    idx_t digit_index = (x - 1) % d;          /* position inside the number */
    idx_t pow_digit   = POW10[d - 1 - digit_index];
    return (uint8_t)((number / pow_digit) % 10);
}

/* a(n) as defined in the OEIS entry. */
seq_t a(idx_t n)
{
    if (n < 2)                 /* a(1) is infinite; return 0 for n<2 */
        return (seq_t)0;

    idx_t z = 0;
    for (;;) {
        idx_t pos1 = z + 1;          /* position in the leading prefix   */
        idx_t pos2 = n + z;          /* position in the shifted suffix    */
        if (cham_digit(pos1) != cham_digit(pos2))
            break;
        ++z;
    }
    return (seq_t)z;
}
