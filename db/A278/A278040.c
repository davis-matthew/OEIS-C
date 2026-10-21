/*
 * A278040  The tribonacci representation of a(n) is obtained by appending 0,1
 *          to the tribonacci representation of n (cf. A278038).
 * https://oeis.org/A278040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A003145(n+1) - 1 * This implementation computes a(n) by
 *            generating the tribonacci representation of n, * appending the
 *            digits “0,1” to that representation, and evaluating the new *
 *            tribonacci‐encoded number. The result is * * a(n) = T3 + Σ
 *            T_{i+2} * * over all indices i where the represent...
 * Range:     u8 n <= 75, u16 n <= 9582, u32 n <= 9582, u64 n <= 9582,
 *            u128 n <= 9582, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *tribo = NULL;           /* dynamic array of tribonacci numbers   */
static size_t tribo_len = 0;          /* number of elements currently stored  */
static size_t tribo_cap = 0;          /* current capacity of the array        */

/* grow the tribonacci array when needed */
static void tribo_grow(void)
{
    size_t new_cap = tribo_cap ? tribo_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(tribo, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* not enough memory */
    tribo = tmp;
    tribo_cap = new_cap;
}

/* initialise the array with the first three tribonacci numbers */
static void tribo_init(void)
{
    if (!tribo) {
        tribo_cap = 8;
        tribo_len = 3;
        tribo = (seq_t *)calloc(tribo_cap, sizeof(seq_t));
        if (!tribo) abort();
        tribo[0] = 0;
        tribo[1] = 0;
        tribo[2] = 1;
    }
    /* make sure that T3 (value 1) is present */
    if (tribo_len <= 3) {
        while (tribo_len <= 3) {
            if (tribo_len == tribo_cap) tribo_grow();
            tribo[tribo_len] = tribo[tribo_len-1] + tribo[tribo_len-2] + tribo[tribo_len-3];
            tribo_len++;
        }
    }
}

/* ensure the array contains an entry up to index “idx” (inclusive) */
static void tribo_ensure(size_t idx)
{
    tribo_init();
    while (tribo_len <= idx) {
        if (tribo_len == tribo_cap) tribo_grow();
        tribo[tribo_len] = tribo[tribo_len-1] + tribo[tribo_len-2] + tribo[tribo_len-3];
        tribo_len++;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    tribo_init();

    /* Special case n == 0 */
    if (n == 0) {
        tribo_ensure(3);               /* T3 is required */
        return tribo[3];               /* equals 1 */
    }

    /* 1. grow tribonacci numbers until T[last] > n */
    size_t i = tribo_len - 1;
    while (tribo[i] <= n && i + 1 == tribo_len) {
        tribo_ensure(tribo_len);        /* grows by one element */
        i = tribo_len - 1;
    }

    /* 2. find the highest index s such that T[s] ≤ n  (s ≥ 3) */
    while (i > 2 && tribo[i] > n) i--;
    size_t s = i;

    /* 3. make sure we have T[s+2] needed for the result */
    tribo_ensure(s + 2);

    /* 4. evaluate a(n) */
    seq_t value = tribo[3];            /* the fixed trailing '1' gives T3 */
    seq_t rem   = (seq_t)n;

    for (size_t k = s; ; --k) {
        if (rem >= tribo[k]) {
            rem -= tribo[k];
            value += tribo[k + 2];
        }
        if (k == 3) break;             /* stop after processing index 3 */
    }

    return value;
}
