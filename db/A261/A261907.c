/*
 * A261907  Numbers that are the sum of two nonzero palindromes but are not
 *          palindromes themselves.
 * https://oeis.org/A261907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number that is the sum of two nonzero palindromes but is
 *            not a palindrome itself. The implementation enumerates all
 *            palindromes up to a fixed limit, marks all sums of two such
 *            palindromes, and collects the non‑palindromic ones in increasing
 *            order. The first 24 terms match the OEIS data. Th...
 * Range:     u8 n <= 212, u16 n <= 7814, u32 n <= 7814, u64 n <= 7814,
 *            u128 n <= 7814, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* utility: test whether x is a decimal palindrome.  x fits in uint64_t
   because all numbers we deal with are below LIMIT. */
static inline bool is_palindrome_uint64(uint64_t x)
{
    uint64_t rev = 0, tmp = x;
    while (tmp > 0) {
        rev = rev * 10 + (tmp % 10);
        tmp /= 10;
    }
    return rev == x;
}

/* maximum value to precompute up to; trades memory vs. desired range
   ; 2 000 000 gives more than enough terms for n up to 10 000. */
#define LIMIT 2000000

/* static cache data */
static bool   *represented   = NULL;   /* size LIMIT+1, initialized to false   */
static bool   *is_pal        = NULL;   /* size LIMIT+1, true for palindromes   */
static uint32_t *results     = NULL;   /* array of sequence values (up to LIMIT) */
static size_t   results_size = 0;      /* number of terms found               */
static bool     initialized  = false;

/* initialise all static arrays and precompute the sequence */
static void init_cache(void)
{
    if (initialized) return;
    initialized = true;

    /* allocate storage */
    represented = (bool*)calloc(LIMIT + 1, sizeof(bool));
    is_pal      = (bool*)calloc(LIMIT + 1, sizeof(bool));
    results     = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!represented || !is_pal || !results) {
        /* allocation failed – leave data uninitialised (unlikely in contest) */
        exit(EXIT_FAILURE);
    }

    /* identify palindromes up to LIMIT */
    size_t pal_count = 0;
    uint32_t **pal_list = (uint32_t**)malloc((LIMIT + 1) * sizeof(uint32_t*)); /* temporary pointer list */
    if (!pal_list) exit(EXIT_FAILURE);
    for (uint32_t i = 1; i <= LIMIT; ++i) {
        bool p = is_palindrome_uint64((uint64_t)i);
        is_pal[i] = p;
        if (p) {
            pal_list[pal_count++] = (uint32_t*)i; /* store as pointer for later iteration */
        }
    }
    /* build sums of two palindromes */
    for (size_t ia = 0; ia < pal_count; ++ia) {
        uint32_t a = (uint32_t)(uintptr_t)pal_list[ia];
        for (size_t ib = 0; ib < pal_count; ++ib) {
            uint32_t b = (uint32_t)(uintptr_t)pal_list[ib];
            uint32_t sum = a + b;
            if (sum > LIMIT) break;   /* pal_list sorted, so further b will only increase */
            represented[sum] = true;
        }
    }
    free(pal_list);

    /* collect the answers: non‑palindromic numbers that are representable */
    for (uint32_t i = 1; i <= LIMIT; ++i) {
        if (represented[i] && !is_pal[i]) {
            results[results_size++] = i;
        }
    }
    /* we could shrink results array here, but keeping it large is fine */
}

seq_t a(idx_t n)
{
    init_cache();                      /* ensure data ready */
    if (n == 0) return 0;              /* undefined; safety */
    if ((size_t)n > results_size) {
        /* n beyond precomputed range – could extend limit here,
           but for typical contest sizes this should not happen. */
        return 0;
    }
    return (seq_t)results[(size_t)n - 1];
}
