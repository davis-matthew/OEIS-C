/*
 * A241909  Self-inverse permutation of natural numbers: a(1)=1, a(p_i) = 2^i,
 *          and if n = p_i1 * p_i2 * p_i3 * ... * p_{ik-1} * p_ik, where p's
 *          are primes, with their indexes are sorted into nondescending
 *          order: i1 <= i2 <= i3 <= ... <= i_{k-1} <= ik, then a(n) =
 *          2^(i1-1) * 3^(i2-i1) * 5^(i3-i2) * ... * p_k^...
 * https://oeis.org/A241909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) for OEIS sequence A241909 * If n = p_1^e1 * p_2^e2 * ... *
 *            p_k^ek with p_i ascending, * let i_j be the index of p_j in the
 *            prime sequence. * a(n) = 2^(i1-1) * 3^(i2-i1) * 5^(i3-i2) * ...
 *            * p_k^(1 + (ik-i_{k-1})). * For a prime p_i, this reduces to
 *            a(p_i) = 2^i
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 130, u64 n <= 310,
 *            u128 n <= 718, big any n
 * Helpers:   compute_pi, factor_u64, get_prime, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */
/* Include the utility prototypes that we need.                     */
/* ------------------------------------------------------------------ */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/compute_pi.h"

/* ------------------------------------------------------------------ */
/* The required function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle the trivial cases directly. */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;   /* 2 is p_1 → 2^1 = 2 */

    /* Factor n: pr[] holds distinct primes, ex[] holds their exponents. */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* If n is a single prime, use the simplified rule a(p_i)=2^i. */
    if (cnt == (size_t)1 && ex[0] == (uint64_t)1) {
        idx_t pi = (idx_t)compute_pi(pr[0]);      /* index of the prime */
        return seq_pow((seq_t)2, pi);
    }

    /* Compute how many total prime occurrences there are. */
    size_t total = 0;
    for (size_t i = 0; i < cnt; ++i) {
        total += (size_t)ex[i];
    }

    /* Allocate an array to hold the indices i_j with multiplicity. */
    idx_t *indices = (idx_t *)malloc(total * sizeof(idx_t));
    if (!indices) return (seq_t)0;  /* allocation failure, unlikely */

    /* Fill indices in ascending order (factor_u64 gives primes sorted). */
    size_t pos = 0;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t pi = (idx_t)compute_pi(pr[i]);   /* index of this prime */
        for (uint64_t e = 0; e < ex[i]; ++e) {
            indices[pos++] = pi;
        }
    }

    /* Compute the product according to the formula. */
    seq_t result = (seq_t)1;
    for (size_t j = 0; j < total; ++j) {
        uint32_t curr_prime = get_prime((idx_t)(j + (idx_t)1));  /* 1‑based */

        idx_t exp;
        if (j == (size_t)0) {
            /* First exponent i1-1. */
            exp = indices[0] - (idx_t)1;
        } else if (j == total - (size_t)1) {
            /* Last exponent 1 + (ik-i_{k-1}). */
            exp = (idx_t)1 + indices[j] - indices[j - 1];
        } else {
            /* Middle exponents i_j - i_{j-1}. */
            exp = indices[j] - indices[j - 1];
        }

        seq_t term = seq_pow((seq_t)curr_prime, exp);
        result = seq_mul(result, term);
    }

    free(indices);
    return result;
}
