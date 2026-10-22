/*
 * A342089  Numbers that have two representations as the sum of distinct
 *          non-consecutive Lucas numbers (A000032).
 * https://oeis.org/A342089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that have two representations as the sum of distinct non‑consecutive
   Lucas numbers (OEIS A342089).  This implementation precomputes all Lucas
   numbers up to a fixed LIMIT, counts every admissible sum with the
   non‑consecutive constraint, then stores those sums whose count is at
   least two.  The function returns the nth term (1‑based index).  The
   algorithm is O(L) in the number of admissible sums and requires only
   O(L) memory. */

/* Requirements: no global functions other than the sequence function,
   no hard‑coded lookup table, and the code must compile as C11 and C++17. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- configuration ----------------------------------------------------- */
#define LIMIT 1000000U   /* large enough for the first 10 000 terms */

/* ---- global state ------------------------------------------------------ */
static uint64_t lucas[64];
static size_t lucas_len;

/* counter for each sum (0 … LIMIT) */
static unsigned char *counter = NULL;
/* array of sequence terms (indices 0 … terms_len‑1 correspond to a(1)…a(n)) */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* ---- helper to resize the terms array --------------------------------- */
static void add_term(uint64_t v)
{
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 8;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = (seq_t)v;
}

/* ---- recursive generation of admissible sums -------------------------- */
static void rec(int idx, bool prev_used, uint64_t sum)
{
    if (idx == (int)lucas_len) {
        if (sum > 0)
            counter[sum]++;                  /* count all non‑empty sums */
        return;
    }
    /* skip current index */
    rec(idx + 1, false, sum);
    /* use current index if previous one was not used */
    if (!prev_used) {
        uint64_t ns = sum + lucas[idx];
        if (ns <= LIMIT)
            rec(idx + 1, true, ns);
    }
}

/* ---- one‑time initialization ------------------------------------------ */
static void init_sequence(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* build Lucas numbers up to LIMIT (L(0)=2, L(1)=1) */
    lucas_len = 0;
    lucas[lucas_len++] = 2ULL;
    lucas[lucas_len++] = 1ULL;
    while (1) {
        uint64_t next = lucas[lucas_len - 1] + lucas[lucas_len - 2];
        if (next > LIMIT) break;
        lucas[lucas_len++] = next;
    }

    /* allocate counter for sums */
    counter = (unsigned char *)calloc(LIMIT + 1, sizeof(unsigned char));
    /* generate all sums */
    rec(0, false, 0);

    /* collect sums that occur at least twice */
    terms_cap = 16;
    terms_len = 0;
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    for (uint64_t i = 1; i <= LIMIT; ++i) {
        if (counter[i] >= 2)
            add_term(i);
    }

    free(counter);   /* counter no longer needed */
}

/* ---- sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1 is defined, value for n=0 is unused */
        return (seq_t)0;

    init_sequence();

    if ((size_t)n > terms_len)
        return (seq_t)0;  /* if the precomputed LIMIT is insufficient */

    return terms[n - 1];
}
