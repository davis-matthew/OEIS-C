/*
 * A309455  Number of squarefree parts in the partitions of n into 3 parts.
 * https://oeis.org/A309455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..floor(n/3)} Sum_{i=j..floor((n-j)/2)} (mu(i)^2
 *            + mu(j)^2 + mu(n-i-j)^2)
 * Range:     u8 n <= 38, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Möbius square (μ(m)^2) is 1 for square‑free m, 0 otherwise. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static cache for the square‑free test up to the highest n seen */
static uint8_t *squarefree = NULL;   /* 1 = square‑free, 0 = not */
static size_t sq_cap = 0;            /* array size */

/* Recompute square‑free table up to n (inclusive). */
static void recompute_squarefree(idx_t n)
{
    size_t new_cap = (size_t)n + 1;
    uint8_t *new_arr = (uint8_t *)calloc(new_cap, sizeof(uint8_t));
    if (!new_arr) abort();           /* out of memory */

    /* All numbers are initially flagged as square‑free. */
    for (size_t i = 0; i < new_cap; ++i) new_arr[i] = 1;
    /* 0 and 1 are considered square‑free. */
    if (new_cap > 0) new_arr[0] = 1;
    if (new_cap > 1) new_arr[1] = 1;

    /* Mark numbers that contain a squared prime factor. */
    for (idx_t p = 2; p * p <= n; ++p) {
        idx_t p2 = p * p;
        for (idx_t m = p2; m <= n; m += p2) new_arr[(size_t)m] = 0;
    }

    /* Replace the old table. */
    free(squarefree);
    squarefree = new_arr;
    sq_cap = new_cap;
}

/* Ensure the square‑free table covers indices 0..n. */
static void ensure_squarefree(idx_t n)
{
    if ((size_t)n + 1 <= sq_cap) return;
    recompute_squarefree(n);
}

/* OEIS sequence A309455. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;

    ensure_squarefree(n);

    seq_t total = 0;
    idx_t j_limit = n / 3;
    for (idx_t j = 1; j <= j_limit; ++j) {
        idx_t i_max = (n - j) / 2;
        for (idx_t i = j; i <= i_max; ++i) {
            idx_t k = n - i - j;
            total += (seq_t)squarefree[(size_t)i];
            total += (seq_t)squarefree[(size_t)j];
            total += (seq_t)squarefree[(size_t)k];
        }
    }
    return total;
}
