/*
 * A265562  Smallest base-4 palindrome m >= n, written in base 10.
 * https://oeis.org/A265562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 80 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest base‑4 palindrome ≥ n, expressed in base 10. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Advance the palindrome stored in *digits to the next larger
   base‑4 palindrome.  *len receives the new length (may increase). */
static void next_palindrome(uint8_t **digits, size_t *len)
{
    size_t l = *len;
    int i = (int)(l - 1) / 2;      /* left middle index (or the lower middle) */
    int j = (int)(l / 2);          /* right middle index (same for odd length) */
    int carry = 1;                 /* increment by one */
    uint8_t *d = *digits;

    while (i >= 0 && carry) {
        int sum = d[i] + carry;
        carry = sum / 4;
        d[i] = sum % 4;
        d[j] = d[i];               /* maintain the palindrome symmetry */
        i--; j++;
    }

    if (carry) {
        /* overflow: create a longer palindrome 100...001 (base‑4) */
        size_t newlen = l + 1;
        uint8_t *newd = (uint8_t*)malloc(newlen);
        if (!newd) abort();        /* unrecoverable memory failure */
        newd[0] = 1;
        for (size_t k = 1; k < newlen; ++k)
            newd[k] = 0;
        newd[newlen - 1] = 1;
        free(d);
        *digits = newd;
        *len = newlen;
    }
}

/* a(n) returns the smallest integer m ≥ n such that m is a palindrome
   when written in base 4.  The result is given in ordinary decimal
   representation (i.e., as a seq_t). */
seq_t a(idx_t n)
{
    /* Handle the trivial case directly. */
    if (n == 0)
        return 0;

    /* ----- 1. Convert n into base‑4 digits (most significant first). ----- */
    idx_t tmp = n;
    size_t len = 0;
    do {
        ++len;
        tmp /= 4;
    } while (tmp > 0);

    uint8_t *dig = (uint8_t*)malloc(len);
    if (!dig) abort();
    tmp = n;
    for (size_t i = len; i-- > 0;) {
        dig[i] = (uint8_t)(tmp % 4);
        tmp /= 4;
    }

    /* ----- 2. Build the smallest palindrome of the same length. ----- */
    uint8_t *pal = (uint8_t*)malloc(len);
    if (!pal) abort();
    memcpy(pal, dig, len);
    for (size_t i = 0; i < len / 2; ++i)
        pal[len - 1 - i] = pal[i];
    free(dig);   /* dig is no longer needed */

    /* ----- 3. Convert the palindrome digits to a seq_t value. ----- */
    seq_t cand = 0;
    for (size_t i = 0; i < len; ++i)
        cand = cand * 4 + pal[i];

    /* ----- 4. If the candidate is below n, keep generating the next
              palindrome until it reaches or exceeds n. ----- */
    seq_t threshold = (seq_t)n;
    while (cand < threshold) {
        next_palindrome(&pal, &len);
        cand = 0;
        for (size_t i = 0; i < len; ++i)
            cand = cand * 4 + pal[i];
    }

    free(pal);
    return cand;
}
