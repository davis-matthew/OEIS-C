/*
 * A339879  Numbers k for which k-1 is a multiple of A053575(k) [the odd part
 *          of phi(k)].
 * https://oeis.org/A339879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k such that (k-1) is a multiple
 *            of the odd part of φ(k) (Euler’s totient of k). The odd part is
 *            obtained with odd_part( φ(k) ). This implementation generates
 *            terms on demand and memoises them, so repeated calls with
 *            arbitrary n are fast
 * Range:     u8 n <= 99, u16 n <= 7018, u32 n <= 9681, u64 n <= 9681,
 *            u128 n <= 9681, big any n
 * Helpers:   odd_part, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary utilities from the racing‐sequence framework */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/phi.h"          /* seq_t phi(idx_t m); */
#include "../utils/odd_part.h"     /* seq_t odd_part(seq_t x); */

/* Internal memoisation storage */
static seq_t *terms     = NULL;  /* array of computed terms   */
static size_t terms_cap = 0;     /* allocated capacity */
static size_t terms_cnt = 0;     /* number of stored terms   */
static idx_t   k_last   = 0;     /* largest k processed */

/* Ensure the terms array can hold at least 'need' elements. */
static void ensure_terms_cap(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* out of memory */
    terms      = tmp;
    terms_cap  = new_cap;
}

/* Add a new term to the memoised list. */
static void add_term(idx_t k)
{
    ensure_terms_cap(terms_cnt + 1);
    terms[terms_cnt++] = (seq_t)k;
}

/* Generate terms until we have at least 'target' terms. */
static void generate_terms_upto(size_t target)
{
    while (terms_cnt < target) {
        ++k_last;
        seq_t odd_phi = odd_part( phi(k_last) );
        /* (k-1) % odd_phi == 0 */
        if ( odd_phi != 0 && ((seq_t)k_last - 1) % odd_phi == 0 )
            add_term(k_last);
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* undefined for n=0; return 0 as placeholder */
    generate_terms_upto((size_t)n);
    return terms[n - 1];
}
