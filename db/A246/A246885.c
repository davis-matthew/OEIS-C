/*
 * A246885  Those n for which the coefficients of x^n in the reciprocal of
 *          1+x+x^8+...+x^(i^3)+... are odd.
 * https://oeis.org/A246885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer m (m ≥ 0) such that the number of
 *            compositions of m into cubes is odd
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the reciprocal of 1 + x + x^8 + x^27 + …   */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state for the memoisation */
static size_t parity_cap = 0;     /* capacity of the parity array          */
static uint8_t *parity = NULL;    /* parity[i] = 0/1 (even/odd composition) */
static idx_t current_limit = 0;   /* largest i that has been processed     */

static size_t odd_cap = 0;        /* capacity of the odd index list        */
static seq_t *odd_vals = NULL;   /* odd_vals[k] = k‑th odd index          */
static size_t odd_count = 0;      /* number of odd indices already stored  */

/* initialise the static arrays once */
static void init(void)
{
    if (parity != NULL)            /* already initialised */
        return;

    /* start with a small capacity and grow as needed */
    parity_cap = 16;
    parity = (uint8_t *)malloc(parity_cap * sizeof(uint8_t));
    if (parity == NULL)
        abort();
    parity[0] = 1;                /* one composition of 0 (empty) */
    current_limit = 0;

    odd_cap = 16;
    odd_vals = (seq_t *)malloc(odd_cap * sizeof(seq_t));
    if (odd_vals == NULL)
        abort();
    odd_vals[0] = (seq_t)0;       /* 0 is the first odd index       */
    odd_count = 1;
}

/* append a value to odd_vals, growing the array if necessary */
static void append_odd(idx_t val)
{
    if (odd_count >= odd_cap) {
        size_t new_cap = odd_cap ? odd_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(odd_vals, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        odd_vals = tmp;
        odd_cap = new_cap;
    }
    odd_vals[odd_count++] = (seq_t)val;
}

/* ensure parity array can hold index i (i <= new_limit) */
static void ensure_parity_size(idx_t i)
{
    while (i >= parity_cap) {
        size_t new_cap = parity_cap ? parity_cap * 2 : 16;
        uint8_t *tmp = (uint8_t *)realloc(parity, new_cap * sizeof(uint8_t));
        if (tmp == NULL)
            abort();
        parity = tmp;
        parity_cap = new_cap;
    }
}

/* main function required by the task */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;       /* offset is 1; unused otherwise */

    init();

    /* extend the precomputed data until we have at least n odd indices */
    while (odd_count < n) {
        idx_t i = current_limit + 1;
        uint8_t val = 0;

        /* parity[i] = XOR of parity[i - k^3] for all k^3 <= i */
        for (idx_t k = 1; ; ++k) {
            idx_t cube = (idx_t)k * k * k;
            if (cube > i)
                break;
            val ^= parity[i - cube];
        }

        ensure_parity_size(i);
        parity[i] = val;
        ++current_limit;

        if (val)           /* odd number of compositions */
            append_odd(i);
    }

    return odd_vals[n - 1];
}
