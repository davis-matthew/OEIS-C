/*
 * A246700  Table read by rows: trajectories under iteration of Carmichael's
 *          lambda function (cf. A002322).
 * https://oeis.org/A246700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1272, u16 n <= 5462, u32 n <= 5462, u64 n <= 5462,
 *            u128 n <= 5462, big any n
 * Helpers:   gcd_u64, lcm_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Table read by rows: trajectories under iteration of Carmichael's lambda function (A246700) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd_u64.h"
#include "../utils/lcm_u64.h"

/* Euclidean algorithm for uint64_t (used internally by λ computation) */

/* Integer power for uint64_t with small exponent */
static inline uint64_t pow_u64(uint64_t base, uint64_t exp)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        exp >>= 1;
        base *= base;
    }
    return res;
}

/* λ(n) for a uint64_t argument, computed by trial division */
static seq_t lambda_u64(uint64_t n)
{
    if (n <= 2) return (seq_t)1;        /* λ(1)=1, λ(2)=1 */

    uint64_t result = 1;
    uint64_t temp   = n;

    /* factor 2 */
    if ((temp & 1) == 0) {
        uint64_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        uint64_t lambda_pe;
        if (e == 1)            /* λ(2) = 1 */
            lambda_pe = 1;
        else if (e == 2)       /* λ(4) = 2 */
            lambda_pe = 2;
        else                   /* λ(2^e) = 2^(e-2) for e ≥ 3 */
            lambda_pe = 1ULL << (e - 2);
        result = lcm_u64(result, lambda_pe);
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* λ(p^e) = p^(e-1) * (p-1)  (p odd) */
            uint64_t lambda_pe = pow_u64(p, e - 1) * (p - 1);
            result = lcm_u64(result, lambda_pe);
        }
    }

    /* remaining prime factor */
    if (temp > 1) {
        uint64_t lambda_pe = temp - 1;
        result = lcm_u64(result, lambda_pe);
    }

    return (seq_t)result;
}

/* λ function for arbitrary seq_t values.
   Works for values that fit in uint64_t (which is sufficient for this sequence). */
static seq_t lambda_of(seq_t n)
{
    return lambda_u64((uint64_t)n);
}

/* Compute the length of the λ‑trajectory starting at start (including start). */
static idx_t trajectory_length(idx_t start)
{
    idx_t len = 1;
    seq_t cur = (seq_t)start;
    while (cur != (seq_t)1) {
        cur = lambda_of(cur);
        ++len;
    }
    return len;
}

/* Helper to grow a seq_t array */
static void grow_seq(seq_t **array, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 16 : *cap;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(*array, new_cap * sizeof(seq_t));
    *array = tmp;
    *cap   = new_cap;
}

/*
 * External function exposing the A246700 sequence.
 */
seq_t a(idx_t n)
{
    /* static state shared between calls */
    static seq_t *seq = NULL;     /* flattened list of terms */
    static size_t  seq_cap = 0;
    static size_t  seq_len = 0;   /* number of terms currently stored */
    static idx_t   next_row = 1;  /* index of the next row that has not yet been generated */

    if (n == 0) return (seq_t)0;  /* undefined for index 0, but allowed */

    /* Generate rows until we have at least n terms */
    while (seq_len < n) {
        /* Generate the next row */
        idx_t len    = trajectory_length(next_row);
        size_t new_needed = seq_len + (size_t)len;
        grow_seq(&seq, &seq_cap, new_needed);

        seq_t cur = (seq_t)next_row;
        for (idx_t i = 0; i < len; ++i) {
            seq[seq_len++] = cur;
            if (i + 1 < len) cur = lambda_of(cur);
        }

        ++next_row;
    }

    return seq[n - 1];
}
