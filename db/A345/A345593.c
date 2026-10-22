/*
 * A345593  Numbers that are the sum of nine fourth powers in nine or more
 *          ways.
 * https://oeis.org/A345593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8745, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers that are the sum of nine fourth powers in at least nine unordered
   ways.  Computed by enumerating all 9‑tuples of integers 1..MAX_BASE,
   counting how many represent a given sum, and keeping those sums with
   at least nine representations. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_BASE 25   /* largest fourth‑power index to use */

static idx_t *pow4 = NULL;      /* pow4[i] = i⁴ (uint64_t)            */
static idx_t *count = NULL;     /* count[sum] = number of unordered 9‑tuples giving sum */
static idx_t *terms = NULL;     /* sorted list of sums with count≥9       */
static size_t terms_size = 0;   /* number of terms stored              */
static bool initialized = false;

/* Recursively generate all non‑decreasing 9‑tuples and count sums */
static void gen(int depth, int start, idx_t cur_sum)
{
    if (depth == 9) {
        count[cur_sum]++;          /* reached a complete 9‑tuple */
        return;
    }
    for (int i = start; i <= MAX_BASE; ++i) {
        gen(depth + 1, i, cur_sum + pow4[i]);
    }
}

/* One‑time initialization building all tables */
static void init(void)
{
    /* allocate and compute fourth powers */
    pow4 = (idx_t *)calloc(MAX_BASE + 1, sizeof(idx_t));
    for (int i = 1; i <= MAX_BASE; ++i) {
        pow4[i] = (idx_t)i * (idx_t)i * (idx_t)i * (idx_t)i;
    }

    /* maximum possible sum */
    idx_t max_sum = 9ULL * pow4[MAX_BASE];

    /* allocate the count array */
    count = (idx_t *)calloc((size_t)(max_sum + 1), sizeof(idx_t));

    /* enumerate all unordered 9‑tuples and count them */
    gen(0, 1, 0);

    /* build the list of sums with at least nine representations */
    size_t cap = 1 << 16;   /* initial capacity for terms array */
    terms = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!terms) return;     /* memory failure – should never happen in tests */

    for (idx_t s = 0; s <= max_sum; ++s) {
        if (count[s] >= 9) {
            if (terms_size == cap) {
                cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(terms, cap * sizeof(idx_t));
                if (!tmp) return;   /* memory failure */
                terms = tmp;
            }
            terms[terms_size++] = s;
        }
    }

    /* free temporary storage */
    free(pow4);
    free(count);

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0 */

    if (!initialized) init();      /* compute sequence once */

    if (n > terms_size) return 0;  /* beyond pre‑computed range */

    return (seq_t)terms[n - 1];
}
