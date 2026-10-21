/*
 * A281021  Primes k with (say) j digits that produce two other primes, x and
 *          y, from the sums x = Sum_{i=1..j} (k mod 10^i) and y =
 *          Sum_{i=0..j-1} (floor(k/10^i)).
 * https://oeis.org/A281021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 11, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes OEIS A281021:
   For each n≥1, a(n) is the n‑th prime k such that
   x = Σ_{i=1..j} (k mod 10^i)
   y = Σ_{i=0..j-1} ⌊k/10^i⌋
   are both prime (j = number of decimal digits of k). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>          /* for memcpy in append_value */

/* Utility headers supplied by the build system */
#include "../utils/is_prime.h"          /* bool is_prime(idx_t) */
#include "../utils/is_prime_big.h"     /* bool is_prime_big(seq_t) */

/* Dynamic array helper ----------------------------------------------------- */
static void append_value(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Check whether a prime k satisfies the OEIS condition. ------------------- */
static bool satisfies_k(idx_t k)
{
    /* Extract decimal digits LSB first. */
    uint8_t digits[32];
    idx_t t = k, j = 0;
    while (t > 0) {
        digits[j++] = (uint8_t)(t % 10);
        t /= 10;
    }
    if (j == 0)                /* no prime has 0 digits – defensive */
        return false;

    /* Compute x = sum of suffixes. */
    seq_t suffix = 0ULL;
    seq_t power  = 1ULL;
    seq_t x      = 0ULL;
    for (idx_t i = 0; i < j; ++i) {
        suffix += (seq_t)digits[i] * power;
        x      += suffix;
        power  *= 10ULL;
    }

    /* Compute y = sum of prefixes. */
    seq_t y      = 0ULL;
    seq_t prefix = (seq_t)k;
    while (prefix > 0) {
        y += prefix;
        prefix /= 10ULL;
    }

    return is_prime_big(x) && is_prime_big(y);
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1; undefined for n<1 */

    /* Static cache of discovered terms and the next candidate. */
    static seq_t *terms   = NULL;
    static size_t  tlen   = 0;
    static size_t  tcap   = 0;
    static idx_t   cand   = 2;  /* next number to test for primality */

    while (tlen < n) {
        /* Find next prime. */
        for (;;) {
            if (cand == 2 || (cand > 2 && (cand & 1U) != 0)) {
                if (is_prime(cand))
                    break;
            }
            cand++;
        }

        /* Test the OEIS property. */
        if (satisfies_k(cand))
            append_value(&terms, &tlen, &tcap, (seq_t)cand);

        cand++;   /* advance for next call */
    }

    return terms[n - 1];
}
