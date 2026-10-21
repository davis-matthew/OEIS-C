/*
 * A246452  Let A be the set of all numbers m for which m^2+1 is prime (see
 *          A005574). The sequence lists the pairs of numbers (a, b) such that
 *          every m > 1 included in A can be written in the form m = a + b for
 *          a <= b both in A and a minimum.
 * https://oeis.org/A246452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 93, u32 n <= 93, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   in_A, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes A246452 – minimal decompositions of numbers m with m^2+1 prime.
 * Each pair (a,b) satisfies a+b=m, a<=b, and both a and b are in
 * A={m | m^2+1 is prime}.  The sequence lists the a's and b's
 * alternately: a(2n-1)=a, a(2n)=b.
 *
 * The function builds the list of m in A on demand and memoises
 * the resulting a,b pairs so any call order works.
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime_big.h"   /* declares bool is_prime_big(seq_t n); */
#include "../utils/in_A.h"

seq_t a(idx_t n) {
    static seq_t *A = NULL;          /* numbers m with m^2+1 prime, sorted */
    static size_t A_len = 0, A_cap = 0;
    static seq_t *pair_a = NULL;
    static seq_t *pair_b = NULL;
    static size_t pair_len = 0, pair_cap = 0;

    /* Initialise static data on first call. */
    if (!A) {
        A_cap = 16;
        A = (seq_t *)malloc(A_cap * sizeof(seq_t));
        if (!A) abort();
        A[0] = (seq_t)1;             /* 1^2+1 = 2 prime */
        A_len = 1;

        pair_cap = 16;
        pair_a = (seq_t *)malloc(pair_cap * sizeof(seq_t));
        pair_b = (seq_t *)malloc(pair_cap * sizeof(seq_t));
        if (!pair_a || !pair_b) abort();
    }

    /* How many pairs are required for index n? */
    size_t needed_pairs = (size_t)((n + 1) / 2);
    while (pair_len < needed_pairs) {
        /* Find next m in A. After 1, all further m are even. */
        seq_t next_m;
        if (A_len == 1)
            next_m = (seq_t)2;
        else
            next_m = A[A_len - 1] + (seq_t)2;   /* skip to next even */

        /* Search for prime m^2+1 */
        for (;;) {
            seq_t val = next_m * next_m + (seq_t)1;
            if (is_prime_big(val))
                break;
            next_m += (seq_t)2;
        }

        /* Ensure capacity for A. */
        if (A_len == A_cap) {
            size_t new_cap = A_cap * 2;
            seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            A = tmp;
            A_cap = new_cap;
        }
        A[A_len++] = next_m;

        /* Find minimal a,b ∈ A with next_m = a+b, a ≤ b. */
        seq_t best_a = 0, best_b = 0;
        for (size_t i = 0; i < A_len; ++i) {
            seq_t a_val = A[i];
            if (a_val > next_m / 2) break;      /* a cannot exceed half of m */
            seq_t b_val = next_m - a_val;
            if (b_val < a_val) continue;        /* enforce a ≤ b */

            if (in_A(b_val, A_len, A)) {
                best_a = a_val;
                best_b = b_val;
                break;          /* first found is minimal */
            }
        }

        /* Append the pair to the output arrays. */
        if (pair_len == pair_cap) {
            size_t new_cap = pair_cap * 2;
            seq_t *tmpa = (seq_t *)realloc(pair_a, new_cap * sizeof(seq_t));
            seq_t *tmpb = (seq_t *)realloc(pair_b, new_cap * sizeof(seq_t));
            if (!tmpa || !tmpb) abort();
            pair_a = tmpa;
            pair_b = tmpb;
            pair_cap = new_cap;
        }
        pair_a[pair_len] = best_a;
        pair_b[pair_len] = best_b;
        ++pair_len;
    }

    /* Return the requested element. */
    size_t k = (size_t)((n + 1) / 2) - 1;   /* zero‑based pair index */
    return (n & 1) ? pair_a[k] : pair_b[k];
}
