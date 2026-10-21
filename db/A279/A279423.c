/*
 * A279423  Numbers k such that k^2 has an odd number of digits and the middle
 *          digit is 3.
 * https://oeis.org/A279423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 2563, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by a brute‑force search:
   a(n) = the n-th positive integer k such that k^2 has an odd number
   of decimal digits and the middle digit of k^2 is 3. */

/* The result is memoised in a static array so that repeated calls
   are fast regardless of the order of n.  The routine is safe for
   the provided seq_t and idx_t types. */

#include <stdlib.h>

/* Helper that counts the decimal digits of a value of type seq_t. */
static int digit_len_seq(seq_t x)
{
    int len = 0;
    while (x > 0) {
        x /= 10;
        len++;
    }
    return len;             /* k^2 is never zero, so len>=1 */
}

/* Helper that returns the middle digit of a seq_t number.
   Returns -1 if the number of digits is even (should not happen
   for k^2 that are tested). */
static int middle_digit_seq(seq_t x)
{
    int dlen = digit_len_seq(x);
    if ((dlen & 1) == 0)          /* even length – not needed but safe */
        return -1;
    int m = dlen / 2;              /* m = floor((len-1)/2) */
    while (m-- > 0)
        x /= 10;                   /* remove the least significant m digits */
    return (int)(x % 10);          /* now x = first (m+1) digits, %10 gives middle */
}

/* Public sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;
    static size_t  cap     = 0;
    static idx_t   len     = 0;            /* number of terms already stored */
    static seq_t   cand    = 1;            /* next integer to test */

    /* Initialise storage lazily */
    if (cache == NULL) {
        cap   = 10;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (len < n) {
        /* Scan integers until the next qualifying k is found */
        while (1) {
            seq_t sq = cand * cand;                /* square of the candidate */
            int dlen = digit_len_seq(sq);

            if ((dlen & 1) != 0) {                  /* odd number of digits? */
                int mid = middle_digit_seq(sq);
                if (mid == 3) {
                    /* store the candidate in the cache */
                    if (len == cap) {
                        size_t new_cap = cap * 2;
                        seq_t *tmp = (seq_t *)realloc(cache,
                                                       new_cap * sizeof(seq_t));
                        cache = tmp;
                        cap   = new_cap;
                    }
                    cache[len++] = cand;
                    cand++;                     /* next integer to test */
                    break;                       /* go back to outer loop */
                }
            }
            cand++;                                 /* try next integer */
        }
    }

    return cache[n - 1];
}
