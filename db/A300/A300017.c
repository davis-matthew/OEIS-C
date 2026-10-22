/*
 * A300017  For any k, the cumulative sum a(1) + a(2) + a(3) + ... + a(k)
 *          shows at least a digit 3.
 * https://oeis.org/A300017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1153, u32 n <= 9396, u64 n <= 9396,
 *            u128 n <= 9396, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the smallest unused natural number that, when added
   to the cumulative sum of the first n‑1 terms, produces a number
   containing at least one decimal digit 3.  The sequence is a
   permutation of the natural numbers. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ----- Static storage -------------------------------------------- */
static seq_t *terms = NULL;      /* sequence terms, 0‑based index */
static size_t terms_cap = 0;     /* allocated capacity of terms */
static size_t terms_len = 0;     /* number of terms stored so far */

static bool *used = NULL;        /* used[i] == true iff i has already
                                   appeared in the permutation */
static size_t used_cap = 0;      /* capacity of the used array */

/* cumulative sum of all terms generated up to the current point */
static seq_t sum = 0;

/* ----- Helper functions ------------------------------------------ */

/* Ensure terms has capacity for at least `need` elements. */
static void grow_terms(size_t need) {
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) abort();
    terms = new_terms;
    terms_cap = new_cap;
}

/* Ensure used can index up to `index`.  New entries are zero‑initialized. */
static void grow_used(size_t index) {
    if (index < used_cap) return;
    size_t new_cap = used_cap ? used_cap : 16;
    while (new_cap <= index) new_cap <<= 1;
    bool *new_used = (bool*)realloc(used, new_cap * sizeof(bool));
    if (!new_used) abort();
    /* Zero‑initialize the newly allocated portion */
    memset(new_used + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = new_used;
    used_cap = new_cap;
}

/* Return true if decimal representation of x contains the digit 3.  */
static bool contains_digit3(seq_t x) {
    if (x == 0) return false;
    while (x > 0) {
        if ((x % 10) == 3) return true;
        x /= 10;
    }
    return false;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1 */
    if ((size_t)n > terms_len) {   /* need to generate more terms */
        size_t target = (size_t)n;
        grow_terms(target);
        for (size_t i = terms_len; i < target; ++i) {
            uint64_t m = 1;
            seq_t candidate_sum;
            while (1) {
                grow_used(m);
                if (!used[m]) {
                    candidate_sum = sum + (seq_t)m;
                    if (contains_digit3(candidate_sum))
                        break;
                }
                ++m;
            }
            used[m] = true;
            terms[terms_len++] = (seq_t)m;
            sum += (seq_t)m;
        }
    }
    return terms[n - 1];
}
