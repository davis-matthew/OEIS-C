/*
 * A364699  Numbers k such that 1 + 2^k + 3^k is divisible by 2*k-1.
 * https://oeis.org/A364699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 2021, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS sequence A364699.
   It searches successive integers k and keeps those for which
   1 + 2^k + 3^k is divisible by 2*k-1.  The result is memoised
   so that calls are efficient in any order. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

/* Static storage for the found terms. */
#define MAX_TERMS 20000          /* must be > maximum number of terms needed (e.g. n=10000) */
static seq_t term_buf[MAX_TERMS];
static seq_t *terms = term_buf;
static idx_t terms_len = 0;     /* number of terms currently stored */
static idx_t terms_cap = MAX_TERMS;
static idx_t last_k_seen = 0;   /* last k examined */

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Handle invalid n in a defined way – the caller should not request n==0. */
    if (n <= 0)
        return 0;

    /* Grow the table only once – we are using a fixed buffer. */
    /* Find terms until the n‑th is stored. */
    while (terms_len < n)
    {
        idx_t k = last_k_seen + 1;                     /* candidate k               */
        seq_t d = (seq_t)(2 * (idx_t)k - 1);           /* modulus 2*k-1              */

        /* Compute 2^k mod d and 3^k mod d. */
        seq_t mod2 = mod_pow((seq_t)2, k, d);
        seq_t mod3 = mod_pow((seq_t)3, k, d);

        /* Check the divisibility condition. */
        seq_t sum = (seq_t)1 + mod2 + mod3;            /* 1 + 2^k + 3^k (mod d)     */
        if ((sum % d) == (seq_t)0)
        {
            /* Store the term. */
            if (terms_len < terms_cap)
                terms[terms_len++] = (seq_t)k;
            /* Otherwise, we have run out of buffer – this shouldn't
               happen for the required n. */
        }

        last_k_seen = k;
    }

    return terms[n - 1];
}
