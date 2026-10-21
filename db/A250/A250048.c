/*
 * A250048  Numbers n such that m = floor(n/6) is coprime to n and, if
 *          nonzero, m is also a term of the sequence.
 * https://oeis.org/A250048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 3896, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A250048.
   Each term is the smallest integer n such that
   (1) m = floor(n/6) is coprime to n, and
   (2) if m > 0, then m itself is also a term of the sequence.
   The sequence is generated on demand and memoised.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Memoisation storage */
static seq_t *terms = NULL;          /* array of terms */
static idx_t term_count = 0;         /* number of terms stored (>=1 after init) */
static size_t terms_cap = 0;         /* allocated capacity for terms */

static bool *present = NULL;         /* bitmap: present[i] == true iff i is a term */
static size_t present_cap = 0;       /* allocated capacity for present array */

static idx_t last_checked = 0;       /* largest integer that has been examined */

/* Ensure that the present array can hold at least 'up_to' indices.  */
static void ensure_present_capacity(idx_t up_to)
{
    if ((size_t)present_cap <= (size_t)up_to) {
        size_t new_cap = present_cap ? present_cap : 16;
        while (new_cap <= (size_t)up_to) new_cap *= 2U;
        bool *tmp = (bool *)realloc(present, new_cap * sizeof(bool));
        if (!tmp) exit(EXIT_FAILURE);          /* allocation failed */
        for (size_t i = present_cap; i < new_cap; ++i) tmp[i] = false;
        present = tmp;
        present_cap = new_cap;
    }
}

/* Ensure that the terms array can hold at least 'up_to' elements.  */
static void ensure_terms_capacity(idx_t up_to)
{
    if (term_count < up_to) {
        size_t new_cap = terms_cap ? terms_cap : 8;
        while (new_cap < (size_t)up_to) new_cap *= 2U;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Add a new term to the sequence data structures.  */
static void add_term(seq_t val)
{
    /* Store the term itself */
    ensure_terms_capacity(term_count + 1);
    terms[term_count++] = val;

    /* Mark the value as present; enlarge bitmap if necessary */
    ensure_present_capacity((idx_t)val);
    present[(size_t)val] = true;
}

/* The required interface */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined, but safe */

    /* Initialise on first call */
    if (term_count == 0) {
        ensure_present_capacity(16);
        ensure_terms_capacity(8);
        /* The very first term is 1 */
        add_term((seq_t)1);
        last_checked = 1;
    }

    /* Generate terms until we have at least n of them */
    while (term_count < n) {
        idx_t candidate = last_checked + 1;
        ++last_checked;                       /* next number to test */

        idx_t m = candidate / 6U;              /* m = floor(candidate/6) */

        if (m > 0) {
            seq_t g = gcd((seq_t)m, (seq_t)candidate);
            if (g == 1 && present[(size_t)m]) {
                add_term((seq_t)candidate);
                continue;                     /* processed this term */
            }
        }
        /* If m == 0, only candidate == 1 would satisfy gcd(0,1)==1,
           but 1 was already added, so we do nothing */
    }

    return terms[n - 1];
}
