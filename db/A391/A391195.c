/*
 * A391195  Integers which can be expressed as the sum of two distinct perfect
 *          powers.
 * https://oeis.org/A391195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute integers that can be expressed as the sum of two distinct perfect powers. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Forward declare the routine used by the test harness. */
seq_t a(idx_t n);

/* Internal data for memoization */
static seq_t *terms = NULL;          /* array of computed terms */
static size_t terms_count = 0;       /* number of stored terms */
static size_t terms_capacity = 0;    /* current capacity of array */

/* Helper routine that builds the list of terms up to a given limit L.
   L is the maximum perfect‑power value considered.  The returned
   number of terms is stored in *found.  For speed the sums are
   collected in a boolean array and then copied into the global
   'terms' vector.  */
static void build_terms_up_to_limit(uint64_t limit, size_t *found)
{
    /* Allocate boolean markers for perfect powers. */
    bool *pp_mark   = (bool *)calloc(limit + 1, sizeof(bool));
    bool *sum_mark  = NULL;
    const uint64_t sum_limit = 2 * limit;
    size_t i, j;

    /* Mark perfect powers.  1 is a perfect power (1^b). */
    pp_mark[1] = true;
    for (uint64_t a = 2; a <= limit; ++a) {
        uint64_t val = a * a;          /* a^2 */
        while (val <= limit) {
            pp_mark[val] = true;
            if (val > limit / a) break;  /* next multiplication would overflow or exceed limit */
            val *= a;
        }
    }

    /* Collect unique perfect‑power values. */
    size_t pp_cnt = 0;
    for (i = 1; i <= limit; ++i)
        if (pp_mark[i]) ++pp_cnt;
    uint64_t *pp_vals = (uint64_t *)malloc(pp_cnt * sizeof(uint64_t));
    for (i = 1, j = 0; i <= limit; ++i)
        if (pp_mark[i]) pp_vals[j++] = i;

    /* Allocate boolean array for possible sums. */
    sum_mark = (bool *)calloc(sum_limit + 1, sizeof(bool));

    /* Generate all sums with distinct perfect powers. */
    for (i = 0; i < pp_cnt; ++i) {
        for (j = i + 1; j < pp_cnt; ++j) {
            uint64_t s = pp_vals[i] + pp_vals[j];
            if (s <= sum_limit)
                sum_mark[s] = true;
        }
    }

    /* Copy true entries into global terms vector. */
    *found = 0;
    for (i = 1; i <= sum_limit; ++i) {  /* skip 0,1 as they can't be sums of two distinct powers */
        if (sum_mark[i]) {
            if (terms_count == terms_capacity) {
                terms_capacity = terms_capacity ? terms_capacity * 2 : 128;
                terms = (seq_t *)realloc(terms, terms_capacity * sizeof(seq_t));
            }
            terms[terms_count++] = (seq_t)i;
            ++(*found);
        }
    }

    /* Free temporary storage. */
    free(pp_mark);
    free(pp_vals);
    free(sum_mark);
}

/* Main public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* safety for non‑positive index */

    /* Ensure we have enough terms stored.  If not, keep increasing the
       limit until we reach the required count.  */
    static uint64_t current_limit = 0;
    if (terms_count < (size_t)n) {
        uint64_t limit = current_limit ? current_limit : 2000000; /* start with 2 M */
        size_t found = 0;
        while (terms_count < (size_t)n) {
            /* Reset any previous data. */
            free(terms);
            terms = NULL;
            terms_count = 0;
            terms_capacity = 0;

            build_terms_up_to_limit(limit, &found);
            limits:
            if (terms_count < (size_t)n) {
                limit *= 2;
                goto limits;
            }
            current_limit = limit;
        }
    }

    /* Convert 1‑based index to 0‑based array access. */
    return terms[n - 1];
}
