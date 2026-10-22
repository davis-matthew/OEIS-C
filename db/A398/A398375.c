/*
 * A398375  Numbers divisible by the square of their second smallest distinct
 *          prime factor.
 * https://oeis.org/A398375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = the n-th integer k such that k is divisible by the square
 *            of its second smallest distinct prime factor. If k has fewer
 *            than two distinct prime factors the property does not hold. The
 *            sequence starts 18, 36, 50, 54, 72, ... (see OEIS A398375). This
 *            implementation incrementally searches for...
 * Range:     u8 n <= 25, u16 n <= 6401, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Dynamic array of discovered terms (stored as seq_t).  The array grows
   as more terms are required. */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* The next integer to examine in the search.  We start at 1 but will
   skip small numbers that can never satisfy the condition. */
static uint64_t next_to_test = 1;

/* Grow the dynamic array to at least 'need' elements. */
static void grow_terms(size_t need)
{
    if (terms_cap >= need) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 256;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* fatal: out of memory */
    terms = tmp;
    terms_cap = new_cap;
}

/* Test whether a 64‑bit integer satisfies the sequence property. */
static bool valid_candidate(uint64_t x)
{
    if (x < 6) return false;           /* need at least 2*3*3=18 to satisfy */

    /* Find the smallest distinct prime factor p1. */
    uint64_t n = x;
    uint64_t p1 = 0;

    if (n % 2ULL == 0ULL) {
        p1 = 2ULL;
        while (n % 2ULL == 0ULL) n /= 2ULL;
    } else {
        for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
            if (n % d == 0ULL) {
                p1 = d;
                while (n % d == 0ULL) n /= d;
                break;
            }
        }
        if (p1 == 0ULL) return false;   /* n is prime */
    }

    /* If only one distinct prime factor remains, reject. */
    if (n == 1ULL) return false;

    /* Find the second distinct prime factor p2 (smallest prime factor of n). */
    uint64_t p2 = 0ULL;
    if (n % 2ULL == 0ULL) p2 = 2ULL;
    else {
        for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
            if (n % d == 0ULL) { p2 = d; break; }
        }
        if (p2 == 0ULL) p2 = n;          /* n is prime */
    }

    uint64_t p2sq = p2 * p2;              /* p2^2 fits in 64 bits */
    return (x % p2sq) == 0ULL;
}

/* Ensure that the first 'target' terms are computed and stored. */
static void ensure_terms(idx_t target)
{
    if (terms_len >= (size_t)target) return;

    grow_terms((size_t)target * 2);       /* over‑allocate to reduce future reallocs */

    while (terms_len < (size_t)target) {
        /* Skip non‑qualifying integers. */
        while (!valid_candidate(next_to_test))
            ++next_to_test;

        /* Store the qualifying integer. */
        terms[terms_len++] = (seq_t)next_to_test;
        ++next_to_test;
    }
}

/* Public sequence function.  The sequence is 1‑based; behaviour for n<1
   is undefined. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined; return 0 as a sensible default */

    ensure_terms(n);
    return terms[(size_t)n - 1];
}
