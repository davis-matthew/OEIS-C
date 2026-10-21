/*
 * A294743  Numbers that are the sum of 5 nonzero squares in exactly 9 ways.
 * https://oeis.org/A294743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A294743 lists all integers that can be expressed
   as the sum of five positive squares in exactly nine different
   ways.  A term is only included once, regardless of the order of
   the squared terms.  For n up to 6501 (the proven upper limit) the
   number of such representations is computed by brute force. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

extern seq_t a(idx_t n);

static seq_t *terms = NULL;
static size_t term_len = 0, term_cap = 0;
static const idx_t MAX_SUM = 6501;   /* proven upper bound for the sequence */

/* Helper: extend the terms array if necessary */
static void append_term(seq_t v)
{
    if (term_len == term_cap) {
        size_t newcap = term_cap == 0 ? 16 : term_cap * 2;
        seq_t *newptr = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
        if (!newptr) exit(EXIT_FAILURE);
        terms = newptr;
        term_cap = newcap;
    }
    terms[term_len++] = v;
}

/* Compute all terms once */
static void build_terms(void)
{
    size_t max_val = (size_t)MAX_SUM + 1;
    /* count[s] holds how many ordered 5-tuples of non‑negative integers
       (allowing repeats but in non‑decreasing order) sum to s.  */
    unsigned int *count = (unsigned int *)calloc(max_val, sizeof(unsigned int));
    if (!count) exit(EXIT_FAILURE);

    /* precompute squares */
    const size_t MAX_SQ = (size_t)(MAX_SUM + 1);
    unsigned int *sq = (unsigned int *)malloc((MAX_SQ + 1) * sizeof(unsigned int));
    if (!sq) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= MAX_SQ; ++i) sq[i] = (unsigned int)(i * i);

    /* Enumerate all unordered 5‑tuples a ≤ b ≤ c ≤ d ≤ e, each ≥ 1. */
    for (unsigned int a = 1; a <= MAX_SQ; ++a) {
        unsigned int a2 = sq[a];
        if (a2 > MAX_SUM) break;
        for (unsigned int b = a; b <= MAX_SQ; ++b) {
            unsigned int ab = a2 + sq[b];
            if (ab > MAX_SUM) break;
            for (unsigned int c = b; c <= MAX_SQ; ++c) {
                unsigned int abc = ab + sq[c];
                if (abc > MAX_SUM) break;
                for (unsigned int d = c; d <= MAX_SQ; ++d) {
                    unsigned int abcd = abc + sq[d];
                    if (abcd > MAX_SUM) break;
                    for (unsigned int e = d; e <= MAX_SQ; ++e) {
                        unsigned int sum = abcd + sq[e];
                        if (sum > MAX_SUM) break;
                        count[sum]++;
                    }
                }
            }
        }
    }

    /* Collect all sums with exactly 9 representations */
    for (size_t s = 1; s <= MAX_SUM; ++s)
        if (count[s] == 9)
            append_term((seq_t)s);

    /* Clean up */
    free(count);
    free(sq);
}

/* Public function as required by the build. */
seq_t a(idx_t n)
{
    if (term_len == 0) build_terms();   /* lazy initialization */

    /* n is 1‑based index */
    if (n == 0 || n > term_len) return 0; /* out of bounds – return 0 */
    return terms[n - 1];
}
