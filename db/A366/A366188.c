/*
 * A366188  Positive integers k such that the fifth derivative of the k-th
 *          Bernoulli polynomial B(k, x) contains only integer coefficients.
 * https://oeis.org/A366188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 125, u32 n <= 125, u64 n <= 125,
 *            u128 n <= 125, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A366188 : Number k such that the 5th derivative of the k‑th Bernoulli
 * polynomial has only integer coefficients.
 *
 * The definition uses the prime‑set product
 *     D_5(n) = ∏_{p ≤ (n−3)/(2+((n−3) mod 2)),
 *                     p ∤ (n)_5 ,  s_p(n−4) ≥ p} p .
 *
 * The sequence consists of all n with D_5(n)=1.
 * The property is checked directly for every n up to the known upper
 * bound (904).  The first 24 values are 1 … 24, and the sequence ends
 * at 904.
 *
 * We generate the entire list once and answer queries by index.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static seq_t *seq = NULL;      /* array of sequence terms */
static size_t seq_len = 0;

/* compute whether n satisfies the property (D_5(n)=1) */
static bool is_valid_k(idx_t n)
{
    if (n <= 5)
        return true;

    /* threshold t = (n-3) / (2+((n-3)&1))   (integer division) */
    idx_t rm = (n - 3) & 1;   /* (n-3) mod 2 */
    idx_t denom = 2 + rm;
    idx_t t = (n - 3) / denom;

    /* N-th falling factorial factors: n, n-1, n-2, n-3, n-4 */
    uint64_t f5[5];
    f5[0] = (uint64_t)n;
    f5[1] = (uint64_t)(n - 1);
    f5[2] = (uint64_t)(n - 2);
    f5[3] = (uint64_t)(n - 3);
    f5[4] = (uint64_t)(n - 4);

    /* obtain primes up to the maximum possible t (≤ 450) */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    if (primes == NULL) {
        build_primes_upto(1000, &primes, &primes_cnt);
    }

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > (uint64_t)t)
            break;

        /* check p ∤ (n)_5 */
        bool divides = false;
        for (int j = 0; j < 5; ++j) {
            if (f5[j] % p == 0) {
                divides = true;
                break;
            }
        }
        if (divides)
            continue;  /* gcd(p,(n)_5)>1 , skip */

        /* compute s_p(n-4) */
        idx_t val = n - 4;
        idx_t sum = 0;
        while (val) {
            sum += val % p;
            val /= p;
        }

        if (sum >= p)
            return false;  /* prime factor appears */
    }
    return true;
}

/* public interface required by the harness */
seq_t a(idx_t n)
{
    if (seq == NULL) {   /* first call: build the whole list */
        /* capacity: 1000 > any expected number of terms (< 50) */
        const size_t CAP = 1000;
        seq = (seq_t *)malloc(CAP * sizeof(seq_t));
        seq_len = 0;
        for (idx_t k = 1; k <= 904; ++k) {
            if (is_valid_k(k)) {
                seq[seq_len++] = (seq_t)k;
            }
        }
    }

    if (n == 0 || n > seq_len)
        return 0;   /* undefined beyond the last term */
    return seq[n - 1];
}
