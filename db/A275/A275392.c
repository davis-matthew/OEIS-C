/*
 * A275392  Smallest term in the tribonacci Zeckendorf representation of n.
 * https://oeis.org/A275392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest term in the tribonacci Zeckendorf representation
 *            of n (computed by greedy selection of the largest tribonacci
 *            number not exceeding the current remainder)
 * Range:     u8 n <= 272, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *trib = NULL;       /* tribonacci numbers, trib[0] = 0, trib[1] = 0, trib[2] = 1 */
static size_t trib_len = 0;      /* number of stored entries */
static size_t trib_cap = 0;      /* allocated capacity */

/* Ensure tribonacci array contains all values <= max_needed. */
static void
ensure_tribonacci(idx_t max_needed)
{
    /* Initialize the array if it hasn't been yet. */
    if (trib_len == 0) {
        trib_cap = 8;
        trib = (seq_t *)malloc(sizeof(seq_t) * trib_cap);
        trib[0] = (seq_t)0;
        trib[1] = (seq_t)0;
        trib[2] = (seq_t)1;
        trib_len = 3;
    }

    /* Extend the array until the last value exceeds max_needed. */
    while (trib_len < 3 || trib[trib_len - 1] <= (seq_t)max_needed) {
        seq_t next = trib[trib_len - 1] + trib[trib_len - 2] + trib[trib_len - 3];

        /* Resize if necessary. */
        if (trib_len >= trib_cap) {
            trib_cap *= 2;
            trib = (seq_t *)realloc(trib, sizeof(seq_t) * trib_cap);
        }
        trib[trib_len++] = next;
    }
}

/* Find the index of the largest tribonacci number <= value. */
static idx_t
max_index_leq(seq_t value)
{
    idx_t lo = 0, hi = trib_len - 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (trib[mid] <= value)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure we have tribonacci numbers up to n. */
    ensure_tribonacci(n);

    seq_t rem = (seq_t)n;
    seq_t smallest = 0;

    /* Greedy representation: repeatedly take the largest possible tribonacci number. */
    while (rem > (seq_t)0) {
        idx_t idx = max_index_leq(rem);
        /* Skip the leading zeros if the algorithm chooses them. */
        if (idx <= 1)
            idx = 2;
        seq_t val = trib[idx];

        if (smallest == 0 || val < smallest)
            smallest = val;

        rem -= val;
    }

    return smallest;
}
