/*
 * A390225  Positive numbers whose base-3, base-5, and base-7 expansions have
 *          the same digit sum.
 * https://oeis.org/A390225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose base‑3, base‑5 and base‑7
 *            digit sums are equal (computed by a linear search)
 * Range:     u8 n <= 13, u16 n <= 893, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_DIGITS 512

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static seq_t cand = 1;             /* current candidate being examined */

static uint8_t d3[MAX_DIGITS];
static uint8_t d5[MAX_DIGITS];
static uint8_t d7[MAX_DIGITS];
static size_t len3 = 1, len5 = 1, len7 = 1;
static unsigned long long sum3 = 1, sum5 = 1, sum7 = 1;

static bool initialized = false;

/* Increment digit representation and digit sum for a given base. */
static void inc_digits(uint8_t *digits, unsigned long long *sum,
                       size_t *len, unsigned base)
{
    size_t i = 0;
    while (1) {
        if (i == *len) {          /* new most‑significant digit */
            digits[i] = 0;
            (*len)++;
        }
        (*sum)++;                 /* we always add 1 to the sum first */
        digits[i]++;              /* increment digit */
        if (digits[i] == base) {  /* carry needed */
            digits[i] = 0;
            (*sum) -= base;
            i++;
            continue;
        }
        break;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, undefined for n==0 */

    if (!initialized) {
        /* initialise digit arrays and cache once */
        for (size_t i = 0; i < MAX_DIGITS; ++i) {
            d3[i] = d5[i] = d7[i] = 0;
        }
        cand = 1;
        len3 = len5 = len7 = 1;
        d3[0] = d5[0] = d7[0] = 1;
        sum3 = sum5 = sum7 = 1;
        cache = NULL;
        cache_len = 0;
        cache_cap = 0;
        initialized = true;
    }

    /* generate terms until we have n terms cached */
    while (cache_len < (size_t)n) {
        if (sum3 == sum5 && sum5 == sum7) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();      /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        /* advance to next integer */
        cand = cand + 1;
        inc_digits(d3, &sum3, &len3, 3);
        inc_digits(d5, &sum5, &len5, 5);
        inc_digits(d7, &sum7, &len7, 7);
    }

    return cache[n - 1];
}
