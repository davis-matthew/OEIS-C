/*
 * A352508  Catalan-Niven numbers: numbers that are divisible by the sum of
 *          the digits in their representation in terms of the Catalan numbers
 *          (A014418).
 * https://oeis.org/A352508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 748, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A search function for sequence A352508: Catalan‑Niven numbers. 
    It builds the sequence on demand, keeping the list of found
    terms in a static vector and computing Catalan numbers as needed. */

/* Include only standard C headers; all dynamic allocation must be
   cast explicitly to support C++ builds. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Storage for Catalan numbers and the sequence values.               */
/*  All arrays are allocated with malloc/realloc and cast to the        */
/*  appropriate type, so the code compiles as C11 and also as C++17.    */
static seq_t *catalan = NULL;   /* Catalan numbers C_0, C_1, ... */
static size_t cat_cap   = 0;    /* capacity of catalan array   */
static size_t cat_len   = 0;    /* number of stored Catalan nums*/

static seq_t *terms = NULL;    /* sequence terms found so far */
static size_t term_cap = 0;    /* capacity of terms array    */
static size_t term_len = 0;    /* number of stored terms      */

static idx_t next_candidate = 1;    /* next integer to test in sequence */

/* ------------------------------------------------------------------ */
/*  Append the next Catalan number C_{k+1} = C_k * 2*(2k+1)/(k+2).      */
static void add_catalan(void)
{
    /* k is the index of the last stored Catalan number C_k. */
    idx_t k = (idx_t)(cat_len - 1);
    seq_t last = catalan[cat_len - 1];

    seq_t num = last * (seq_t)(2 * (2 * k + 1));
    seq_t den = (seq_t)(k + 2);
    seq_t next = num / den;   /* integer division is exact          */

    if (cat_len == cat_cap) {                /* grow storage if needed */
        size_t new_cap = cat_cap ? cat_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(catalan, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* out of memory */
        catalan = tmp;
        cat_cap = new_cap;
    }
    catalan[cat_len++] = next;
}

/* ------------------------------------------------------------------ */
/*  Ensure that all Catalan numbers <= limit are stored in the array. */
static void ensure_catalan_up_to(seq_t limit)
{
    if (cat_len == 0) {                         /* first element: C_0 = 1 */
        if (cat_cap == 0) {
            cat_cap = 16;
            catalan = (seq_t *)malloc(cat_cap * sizeof(seq_t));
            if (!catalan) abort();
        }
        catalan[0] = (seq_t)1;
        cat_len = 1;
    }
    /* Add further Catalan numbers until the last one exceeds limit. */
    while (cat_len > 0 && catalan[cat_len - 1] <= limit) {
        add_catalan();
    }
}

/* ------------------------------------------------------------------ */
/*  Test whether k is a Catalan‑Niven number:                         */
static bool is_catalan_niven(seq_t k)
{
    if (k == 0) return false;

    /* Make sure all required Catalan numbers are available. */
    ensure_catalan_up_to(k);

    seq_t rem     = k;
    seq_t sum_dig = 0;

    /* Greedy representation: use each Catalan number as many times
       as possible, starting from the largest.                     */
    for (size_t i = cat_len; i-- > 0;) {
        seq_t c = catalan[i];
        if (c > rem) continue;
        seq_t q = rem / c;
        rem -= q * c;
        sum_dig += q;
        if (rem == 0) break;
    }
    return (rem == 0) && (k % sum_dig == 0);
}

/* ------------------------------------------------------------------ */
/*  Public entry point required by the specification.                 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined behaviour for n < 1 */

    /* Generate more terms until we have at least n of them. */
    while (term_len < n) {
        seq_t cand = (seq_t)next_candidate;
        if (is_catalan_niven(cand)) {
            if (term_len == term_cap) {          /* grow terms array */
                size_t new_cap = term_cap ? term_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* out of memory */
                terms = tmp;
                term_cap = new_cap;
            }
            terms[term_len++] = cand;
        }
        ++next_candidate;
    }

    /* Return the requested term (1‑based index). */
    return terms[n - 1];
}
