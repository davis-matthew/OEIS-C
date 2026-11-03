/*
 * A389531  Standard composition numbers of compositions whose maximal runs R
 *          all satisfy length(R) != first(R).
 * https://oeis.org/A389531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Determination of A389531 – “Standard composition numbers of compositions whose
 * maximal runs R all satisfy length(R) != first(R)”.
 * Computation: iterate non‑negative integers k, map each k to a composition
 * via the standard binary ordering described in the OEIS entry, test the
 * property, and collect the qualifying k’s.  a(n) is the n‑th qualifying k
 * (1‑based). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: test whether the composition associated with k satisfies
 * the property that for every maximal run R, length(R) != first(R). */
static bool composition_good(idx_t k)
{
    /* positions of 1 bits in the binary expansion of k, counted from the
     * least significant bit (starting at 1), and a leading 0 is prepended.
     * The maximum number of bits for a 64‑bit integer is 64. */
    int pos[65];
    int pos_len = 1;      /* pos[0] = 0 */
    pos[0] = 0;
    idx_t temp = k;
    int bit_pos = 1;

    while (temp) {
        if (temp & 1U) {
            if (pos_len < 65) pos[pos_len++] = bit_pos;
        }
        temp >>= 1U;
        bit_pos++;
    }

    /* Compute parts as first differences of consecutive positions. */
    int part[65];
    int p_len = pos_len - 1;
    for (int i = 1; i < pos_len; ++i) {
        part[i - 1] = pos[i] - pos[i - 1];
    }

    /* Reverse to obtain the actual composition order. */
    for (int i = 0; i < p_len / 2; ++i) {
        int tmp = part[i];
        part[i] = part[p_len - 1 - i];
        part[p_len - 1 - i] = tmp;
    }

    /* Empty composition is trivially good. */
    if (p_len == 0) return true;

    /* Scan maximal runs and test the condition. */
    int i = 0;
    while (i < p_len) {
        int run_start = i;
        while (i + 1 < p_len && part[i + 1] == part[run_start]) {
            i++;
        }
        int run_len   = i - run_start + 1;
        int first_val = part[run_start];
        if (run_len == first_val) return false;
        i++;   /* move to next part after the run */
    }
    return true;
}

/* The implementation below is compatible with both the C and C++ builds.
 * It does not rely on realloc, allowing `seq_t` to be an arbitrary‑precision
 * integer class.  A static array of 100 000 elements suffices for all
 * required indices (n <= 10000). */
#define MAX_CACHE_SIZE 100000

static seq_t cache[MAX_CACHE_SIZE];
static size_t cache_size = 0;

/* Append a new term to the cache.  It is assumed that `cache_size`
 * is < MAX_CACHE_SIZE. */
static void cache_add(idx_t k)
{
    cache[cache_size++] = (seq_t)k;
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    static idx_t next_k = 0;    /* next k to test for inclusion */

    while (cache_size < n) {
        while (!composition_good(next_k)) {
            next_k++;
        }
        cache_add(next_k);
        next_k++;
    }
    return cache[n - 1];
}
