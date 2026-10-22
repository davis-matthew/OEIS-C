/*
 * A378563  Primes that remain prime if any three of their digits are deleted.
 * https://oeis.org/A378563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search: successive primes are tested for the
   “delete any three digits” property.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
/* Utilities provided by the framework */
#include "../utils/is_prime_big.h"

/* Count decimal digits of a seq_t number (no external helper needed). */
static int seq_digit_len(seq_t n)
{
    if (n == 0) return 1;
    int len = 0;
    while (n > 0) {
        n /= 10;
        ++len;
    }
    return len;
}

/* Return true iff prime p has the mentioned deletion property.      */
static bool property_holds(seq_t p)
{
    /* Extract digits into an array in most‑significant order. */
    int len = seq_digit_len(p);
    if (len < 4) return false;          /* must have at least four digits */

    uint8_t msd[64];                    /* sufficient for 128‑bit numbers */
    seq_t tmp = p;
    for (int i = 0; i < len; ++i) {      /* store reversed */
        msd[i] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    /* Reverse into msd[0..len‑1] */
    for (int i = 0, j = len - 1; i < j; ++i, --j) {
        uint8_t t = msd[i];
        msd[i] = msd[j];
        msd[j] = t;
    }

    /* Enumerate all ways to delete three digits. */
    for (int a = 0; a < len - 2; ++a)
        for (int b = a + 1; b < len - 1; ++b)
            for (int c = b + 1; c < len; ++c) {
                seq_t val = 0;
                for (int i = 0; i < len; ++i) {
                    if (i == a || i == b || i == c) continue;
                    val = val * 10 + msd[i];
                }
                if (!is_prime_big(val)) return false;
            }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

    /* Static storage for memoised terms. */
    static seq_t *terms   = NULL;
    static size_t term_len = 0;
    static size_t term_cap = 0;

    /* Grow capacity helper. */
    if (term_len + 1 > term_cap) {
        size_t new_cap = term_cap ? term_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return 0;       /* allocation failure */
        terms = tmp;
        term_cap = new_cap;
    }

    /* If we already have the requested term, return it. */
    if (term_len >= n) return terms[n - 1];

    /* Search for the next prime that satisfies the property. */
    seq_t candidate = (n == 1) ? 2237 : terms[term_len - 1] + 2; /* start */
    while (term_len < n) {
        /* Ensure candidate is odd and >=2 */
        if (candidate < 2) candidate = 2;
        else if ((candidate & 1) == 0) ++candidate;

        /* Test primality of the candidate. */
        if (!is_prime_big(candidate)) { candidate += 2; continue; }

        if (property_holds(candidate)) {
            terms[term_len++] = candidate;
        }
        candidate += 2;
    }

    return terms[n - 1];
}
