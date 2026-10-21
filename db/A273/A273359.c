/*
 * A273359  Numbers k such that the decimal number concat(4,k) is a square.
 * https://oeis.org/A273359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that the decimal
 *            concatenation "4" followed by k (i.e. 4⋅10^m + k) is a perfect
 *            square. The implementation iterates through successive integer
 *            squares whose leading decimal digit is 4 and extracts the suffix
 *            after that digit, skipping values whose second di...
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 5585, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* cache of computed terms: seq_cache[i] == a(i+1) */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
/* next integer to test whose square is considered in the loop */
static uint64_t next_m = 0;

/* ensure enough storage for at least `need` terms */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? (seq_cap << 1) : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* append a new term to the cache */
static void append_term(seq_t val)
{
    ensure_capacity(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n < 1 */

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        ++next_m;
        unsigned __int128 square128 = (unsigned __int128)next_m * next_m;
        seq_t square = (seq_t)square128;     /* fits into seq_t for the
                                               requested n */

        int d = digit_len(square);           /* number of decimal digits */
        if (d < 2) continue;                /* need at least two digits */

        seq_t pow10_d1 = pow10((idx_t)d - 1);
        seq_t first = square / pow10_d1;    /* leading digit */
        if (first != 4) continue;           /* must start with 4 */

        seq_t pow10_d2 = pow10((idx_t)d - 2);
        seq_t second = (square / pow10_d2) % 10; /* second decimal digit */
        if (second == 0) continue;          /* second digit cannot be zero */

        seq_t k = square - 4 * pow10_d1;    /* remainder after leading 4 */
        append_term(k);
    }

    return seq_cache[n - 1];
}
