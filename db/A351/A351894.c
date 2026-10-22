/*
 * A351894  Numbers that contain only odd digits in their factorial-base
 *          representation.
 * https://oeis.org/A351894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number whose factorial-base representation uses only odd
 *            digits
 * Range:     u8 n <= 12, u16 n <= 272, u32 n <= 9983, u64 n <= 9983,
 *            u128 n <= 9983, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the nth term of OEIS A351894. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, so n>=1 for valid queries */

    /* Find the length of the factorial representation that contains the nth term. */
    idx_t total = 0;        /* sum of counts for all shorter lengths */
    idx_t prod  = 1;        /* product of digit counts for the current length */
    idx_t len    = 0;       /* current length */

    for (len = 1; ; ++len) {
        idx_t count = (len + 1) / 2;      /* number of odd digits allowed at position len */
        prod *= count;
        total += prod;
        if (total >= n) break;
    }

    /* Rank (0‑based) of the term among all numbers of this length. */
    idx_t cumulative_before = total - prod;          /* numbers with length < len */
    idx_t idx_in_len = n - cumulative_before - 1;    /* 0‑based index within this length */

    /* Allocate temporary arrays for prefix products of counts and factorials. */
    size_t sz = (size_t)(len + 1);
    idx_t *pref = (idx_t *)malloc(sz * sizeof(idx_t));
    seq_t *fact = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!pref || !fact) { /* allocation failed; unlikely but guard against it */
        free(pref); free(fact);
        return 0;
    }

    pref[0] = 1;
    fact[0] = 1;
    for (idx_t i = 1; i <= len; ++i) {
        idx_t count = (i + 1) / 2;
        pref[i] = pref[i-1] * count;
        fact[i] = fact[i-1] * (seq_t)i;
    }

    /* Reconstruct the digits from most significant to least significant. */
    seq_t value   = 0;
    idx_t remaining = idx_in_len;
    for (idx_t i = len; i > 0; --i) {
        idx_t block       = pref[i-1];          /* product of lower‑position counts */
        idx_t digit_index = remaining / block;  /* which allowed digit in this place */
        idx_t digit       = 2 * digit_index + 1;/* digits are odd: 1,3,5,... */
        value += fact[i] * (seq_t)digit;
        remaining -= digit_index * block;
    }

    free(pref);
    free(fact);
    return value;
}
