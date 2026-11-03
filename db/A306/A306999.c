/*
 * A306999  Numbers m such that 1 < gcd(m, 21) < m and m does not divide 21^e
 *          for e >= 0.
 * https://oeis.org/A306999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8965 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A306999: numbers m with 1 < gcd(m,21) < m and m not dividing 21^e.
   Computed by scanning integers and selecting those divisible by 3 or 7
   and having at least one other prime factor (i.e., not of the form 3^a*7^b). */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 1;   /* next integer to test */

/* Test whether m satisfies the defining property. */
static bool valid_candidate(seq_t m)
{
    /* Must share a non‑trivial factor with 21 */
    seq_t g = gcd(m, (seq_t)21);
    if (g == (seq_t)1) return false;

    /* Must contain a prime factor other than 3 or 7 */
    seq_t tmp = m;
    while (tmp % (seq_t)3 == (seq_t)0) tmp /= 3;
    while (tmp % (seq_t)7 == (seq_t)0) tmp /= 7;
    return tmp != (seq_t)1;
}

/* Return the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n < 1 */

    size_t N = (size_t)n;

    if (cache_len < N) {
        /* Ensure the array has enough capacity. */
        grow_seq(&cache, &cache_cap, N);

        /* Generate terms until we have N of them. */
        while (cache_len < N) {
            ++next_candidate;                         /* start from 2 */
            if (valid_candidate(next_candidate))
                cache[cache_len++] = next_candidate;
        }
    }

    return cache[N - 1];
}
