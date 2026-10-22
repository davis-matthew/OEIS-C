/*
 * A394543  Numbers k that reach a power of 2 in zero or more iterations of k
 *          -> 2k - (sum of proper factors of 2k).
 * https://oeis.org/A394543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 43, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A394543: Numbers k that reach a power of 2 under
 * the iteration k → 2k – (sum of proper factors of 2k)
 * (proper factors are all divisors of 2k except the divisor 1).
 * The function a(n) generates this sequence on demand.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: check if a sequence value is an exact power of two. */
static inline bool is_power_of_two(seq_t x) {
    return x != 0 && (x & (x - 1)) == 0;
}

/* Helper: sum of all divisors (σ) for a 64‑bit integer.
 * Used only for values that fit into uint64_t.
 */
static inline seq_t sigma_uint64(uint64_t n) {
    seq_t result = 1;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t power = 1;
            uint64_t sum = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= p;
                sum += power;
            }
            result *= sum;
        }
    }
    if (temp > 1) {          /* last prime factor > sqrt(n) */
        result *= (1 + temp);
    }
    return result;
}

/* Test whether a starting value eventually reaches a power of two. */
static bool reaches_power_of_two(seq_t start) {
    seq_t cur = start;
    /* limit the number of iterations to avoid infinite loops */
    for (int steps = 0; steps < 200; ++steps) {
        if (is_power_of_two(cur))
            return true;

        uint64_t m = (uint64_t)(cur * 2ULL);
        seq_t sigma = sigma_uint64(m);
        seq_t tmp = cur * 4ULL + 1ULL;            /* 4*cur + 1   */
        if (sigma > tmp)                          /* underflow */
            return false;
        cur = tmp - sigma;                        /* next value   */
        if (cur == 0)                            /* 0 is not a power of 2 */
            return false;
    }
    return false;      /* fall‑back if cycle too long */
}

/* ------- Storage for the computed sequence ---- */
static seq_t *seq = NULL;          /* array of found terms */
static idx_t seq_len = 0;          /* number of terms stored    */
static size_t seq_cap = 0;         /* current array capacity    */
static idx_t next_candidate = 1;   /* next integer to test     */

/* Grow the storage array if needed. */
static void ensure_capacity(void) {
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!new_seq) abort();    /* out of memory */
        seq = new_seq;
        seq_cap = new_cap;
    }
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n) {
    if (n == 0) return 0;    /* undefined for zero according to the OEIS offset */

    /* Generate terms until we have the n-th term. */
    while (seq_len < n) {
        seq_t k = (seq_t)(next_candidate);
        if (reaches_power_of_two(k)) {
            ensure_capacity();
            seq[seq_len++] = k;
        }
        ++next_candidate;
    }
    return seq[n - 1];
}
