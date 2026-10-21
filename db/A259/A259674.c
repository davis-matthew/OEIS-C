/*
 * A259674  Numbers n such that the sum of the divisors of n equals the third
 *          power of the sum of the digits of n.
 * https://oeis.org/A259674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that σ(n) = (sum of digits of n)^3 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- local helpers ---------- */
static seq_t digit_sum_uint64(uint64_t n) {
    seq_t sum = 0;
    while (n) {
        sum += (seq_t)(n % 10);
        n /= 10;
    }
    return sum;
}

/* global state for the sieve of smallest prime factors */
static uint32_t *spf      = NULL;   /* spf[i] = smallest prime factor of i (i>=2) */
static size_t    spf_cap  = 0;    /* capacity of spf array */
static size_t    spf_len  = 0;    /* highest index that has a correct spf */

/* rebuild the spf array up to new_limit (inclusive) */
static void rebuild_spf_to(size_t new_limit) {
    if (spf_cap < new_limit + 1) {
        /* enlarge the array to the next power of 2 for simplicity */
        size_t new_cap = spf_cap ? spf_cap << 1 : new_limit + 1;
        if (new_cap < new_limit + 1) new_cap = new_limit + 1;
        uint32_t *tmp = (uint32_t *)realloc(spf, sizeof(uint32_t) * (new_cap + 1));
        if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
        spf      = tmp;
        spf_cap  = new_cap;
    }
    /* initialise */
    for (size_t i = 0; i <= new_limit; ++i) spf[i] = 0;
    spf[0] = 0;
    if (new_limit >= 1) spf[1] = 1;
    /* linear sieve */
    for (size_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= new_limit) {
                uint64_t ii = (uint64_t)i * i;
                for (uint64_t j = ii; j <= new_limit; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }
    spf_len = new_limit;
}

/* compute σ(n) using the precomputed spf */
static seq_t sigma_of(uint64_t n) {
    if (n == 1) return (seq_t)1;
    seq_t result = 1;
    uint64_t x = n;
    while (x > 1) {
        uint32_t p = spf[x];
        uint64_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        seq_t term = 1;
        seq_t part = 1;
        for (uint64_t i = 0; i < e; ++i) {
            term *= (seq_t)p;
            part += term;
        }
        result *= part;
    }
    return result;
}

/* dynamic array holding the sequence terms */
static seq_t *solutions = NULL;
static size_t  sol_cap   = 0;
static size_t  sol_len   = 0;

static void add_solution(seq_t val) {
    if (sol_len == sol_cap) {
        size_t new_cap = sol_cap ? sol_cap << 1 : 8;
        seq_t *tmp = (seq_t *)realloc(solutions, sizeof(seq_t) * new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        solutions = tmp;
        sol_cap = new_cap;
    }
    solutions[sol_len++] = val;
}

/* ---------- required function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* behaviour below offset is irrelevant */

    static size_t cur_limit = 0;     /* largest number already tested */
    static size_t block_size = 1000; /* temporary search block */

    while (sol_len < (size_t)n) {
        size_t new_limit = cur_limit + block_size;
        rebuild_spf_to(new_limit);

        for (size_t m = cur_limit + 1; m <= new_limit; ++m) {
            seq_t sigma = sigma_of((uint64_t)m);
            seq_t ds = digit_sum_uint64((uint64_t)m);
            seq_t ds3 = ds * ds * ds;
            if (sigma == ds3) add_solution((seq_t)m);
        }

        cur_limit = new_limit;
        if (sol_len >= (size_t)n) break;
        block_size <<= 1;
        if (block_size == 0) block_size = 1; /* overflow guard */
    }

    return solutions[(size_t)n - 1];
}
