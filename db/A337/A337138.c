/*
 * A337138  Numbers that cannot be written as the sum of 5 or less squares of
 *          composite numbers, allowing repetitions (complement of A055075).
 * https://oeis.org/A337138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 165, u16 n <= 256, u32 n <= 256, u64 n <= 256,
 *            u128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A337138: numbers that cannot be written as the sum of
   5 or fewer squares of composite numbers (complement of A055075). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Maximum value to be inspected – the sequence stops at 1167 (a(256)). */
#define MAXVAL 2000          /* a safe upper bound */
#define SQ_CAP 128           /* upper bound on the number of composite squares */

/* Static storage for the sequence values. */
static seq_t seq_cache[400] = {0};
static size_t seq_len = 0;
static bool initialized = false;

/* Test whether n is prime (n <= 2000). */
static bool is_prime_u32(idx_t n)
{
    if (n < 2) return false;
    for (idx_t d = 2; d * d <= n; ++d)
        if (n % d == 0) return false;
    return true;
}

/* Initialise the cache once. */
static void init_sequence(void)
{
    if (initialized) return;

    /* Determine the integer square root of MAXVAL. */
    idx_t sqrt_max = 0;
    while ((sqrt_max + 1) * (sqrt_max + 1) <= MAXVAL) ++sqrt_max;

    /* Collect composite squares up to MAXVAL. */
    idx_t squares[SQ_CAP];
    size_t sq_cnt = 0;
    for (idx_t i = 4; i <= sqrt_max; ++i) {
        if (!is_prime_u32(i)) {
            if (sq_cnt < SQ_CAP) {
                squares[sq_cnt++] = i * i;
            }
        }
    }

    /* Dynamic programming: minimal number of composite squares for each sum. */
    unsigned char min_sq[MAXVAL + 1];
    const unsigned char INF = 255;
    for (idx_t i = 0; i <= MAXVAL; ++i) min_sq[i] = INF;
    min_sq[0] = 0;

    for (size_t si = 0; si < sq_cnt; ++si) {
        idx_t v = squares[si];
        for (idx_t s = v; s <= MAXVAL; ++s) {
            unsigned char cand = min_sq[s - v];
            if (cand != INF && cand + 1 < min_sq[s])
                min_sq[s] = cand + 1;
        }
    }

    /* Store numbers that cannot be represented with ≤5 composite squares. */
    seq_len = 0;
    for (idx_t x = 1; x <= MAXVAL && seq_len < 400; ++x) {
        if (min_sq[x] > 5) {
            seq_cache[seq_len++] = (seq_t)x;
        }
    }

    initialized = true;
}

/* Public interface required by the build system. */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0 || n > seq_len) return (seq_t)0;  /* safety check */
    return seq_cache[n - 1];
}
