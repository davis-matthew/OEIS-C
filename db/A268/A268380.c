/*
 * A268380  Numbers having fewer prime factors of the form 4*k+1 than of the
 *          form 4*k+3, when counted with multiplicity.
 * https://oeis.org/A268380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th natural number that has more prime
   factors of the form 4k+3 than of the form 4k+1 (counted with
   multiplicity).  The function memoises the terms found so far, but
   does not assume increasing order of calls. */

#include <stdlib.h>
#include <stdint.h>

/* Static storage for the sequence terms.  The capacity grows
   geometrically when new terms are appended. */
static seq_t *terms   = NULL;
static size_t terms_len = 0;   /* number of terms stored */
static size_t terms_cap = 0;   /* allocated capacity */
static idx_t  scanned_limit = 0;   /* highest integer already scanned */

/* Append a new term to the table, enlarging the array if required. */
static void add_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? (terms_cap << 1) : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Return the nth term of A268380.  a(1) = 3.  Calls may arrive in any
   order, and the function caches previously found terms. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for offset 0 */

    /* Generate terms until we have at least n of them. */
    while (terms_len < n) {
        /* Start scanning from the next integer after the last scanned
           one.  This guarantees we do not reprocess numbers. */
        idx_t num = scanned_limit + 1;

        for (;;) {
            idx_t tmp   = num;
            int   cnt1 = 0;   /* prime factors ≡ 1 (mod 4) */
            int   cnt3 = 0;   /* prime factors ≡ 3 (mod 4) */

            /* Strip all factors of 2 (they are ignored for the
               comparison). */
            while ((tmp & 1) == 0) tmp >>= 1;

            /* Trial division for odd primes up to sqrt(tmp). */
            for (idx_t p = 3; p <= tmp / p; p += 2) {
                while (tmp % p == 0) {
                    tmp /= p;
                    if (p % 4 == 1) ++cnt1;
                    else           ++cnt3;   /* p % 4 == 3 */
                }
            }

            /* Remaining factor, if any, is a prime > sqrt(original). */
            if (tmp > 1) {
                if (tmp % 4 == 1) ++cnt1;
                else              ++cnt3;
            }

            /* Include the number if it satisfies the condition. */
            if (cnt3 > cnt1) add_term((seq_t)num);

            /* If we have gathered enough terms, stop scanning. */
            if (terms_len >= n) {
                scanned_limit = num;   /* remember where we stopped */
                break;
            }

            ++num;                      /* try next integer */
        }
    }

    /* Return the requested term (1‑based indexing). */
    return terms[n - 1];
}
