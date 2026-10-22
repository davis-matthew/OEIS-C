/*
 * A353438  Integers m such that the decimal expansion of 1/m contains the
 *          digit 2.
 * https://oeis.org/A353438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) for sequence A353438:  integers m such that the
    decimal expansion of 1/m contains the digit 2.
    The function performs a brute‑force search, starting from m = 1
    and testing each integer until the requested index is reached. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Auxiliary data for detecting the decimal expansion of 1/m. */
static uint32_t *visited = NULL;          /* marks remainders per test   */
static size_t visited_size = 0;           /* capacity of 'visited' array */
static uint32_t visit_token = 1;          /* unique token per call       */

/* Test whether the decimal expansion of 1/m contains the digit 2. */
static int has_digit2(uint64_t m)
{
    if (m == 0) return 0;               /* division by zero is invalid */

    /* Fraction 1/m with m = 1 gives 1.0 – no fractional digits. */
    if (m == 1) return 0;

    /* Resize the visited array if necessary. */
    if (visited_size < (size_t)m) {
        size_t new_size = (size_t)m + 1;
        uint32_t *new_arr = (uint32_t *)realloc(visited, new_size * sizeof(uint32_t));
        if (!new_arr) return 0;          /* out of memory: treat as failure */
        memset(new_arr + visited_size, 0, (new_size - visited_size) * sizeof(uint32_t));
        visited = new_arr;
        visited_size = new_size;
    }

    uint64_t remainder = 1 % m;
    __uint128_t temp = remainder;       /* use 128‑bit for safety */
    uint32_t token = visit_token++;

    for (;;) {
        temp *= 10;
        uint64_t digit = temp / m;
        if (digit == 2) return 1;       /* found the required digit */
        temp %= m;
        remainder = (uint64_t)temp;

        if (remainder == 0) return 0;   /* terminating decimal */

        /* Detect repetition of remainders (cycle). */
        if (visited[remainder] == token) return 0;
        visited[remainder] = token;
    }
}

/* Storage for computed terms. */
static seq_t *terms = NULL;          /* array of terms that satisfy the condition */
static size_t terms_size   = 0;        /* number of terms stored */
static size_t terms_cap     = 0;        /* capacity of the array  */
static uint64_t last_checked = 1;     /* last integer m that was tested */

/* Ensure the terms array can contain at least 'needed' elements. */
static void grow_terms(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_arr) return;              /* out of memory: leave unchanged */
    terms = new_arr;
    terms_cap = new_cap;
}

/* Main accessor for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined below offset, return 0 */

    /* Generate terms until we have at least n of them. */
    while (terms_size < (size_t)n) {
        if (!has_digit2(last_checked)) {
            last_checked++;              /* skip non‑term */
            continue;
        }
        grow_terms(terms_size + 1);      /* ensure capacity */
        terms[terms_size] = (seq_t)last_checked; /* store the term */
        terms_size++;
        last_checked++;
    }
    return terms[n - 1];
}
