/*
 * A250133  Denominator of the harmonic mean of the first n composite numbers.
 * https://oeis.org/A250133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of the harmonic mean of the first n composite
 *            numbers. Specifically: a(n) = denominator of n / Σ_{i=1}^{n}
 *            1/c_i, where c_i is the ith composite number
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 22, u64 n <= 58, u128 n <= 123,
 *            big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    static uint64_t *comps = NULL;
    static size_t comps_cap = 0, comps_len = 0;
    static seq_t S_num = (seq_t)0;
    static seq_t S_den = (seq_t)1;
    static seq_t *results = NULL;
    static size_t results_cap = 0, results_len = 0;

    if (n == 0) return (seq_t)0;

    if ((idx_t)results_len >= n) {
        return results[n-1];
    }

    /* Generate composites and supply terms up to n */
    while (comps_len < n) {
        uint64_t x;
        if (comps_len == 0) {
            x = 4;
        } else {
            x = comps[comps_len-1] + 1;
            while (is_prime(x)) ++x;
        }

        /* Ensure capacity in the composites array */
        if (comps_len >= comps_cap) {
            size_t new_cap = comps_cap ? comps_cap * 2 : 16;
            uint64_t *tmp = (uint64_t *)realloc(comps, new_cap * sizeof(uint64_t));
            if (!tmp) exit(1);
            comps = tmp;
            comps_cap = new_cap;
        }
        comps[comps_len] = x;

        /* Update the running sum of reciprocals S = S_num / S_den */
        seq_t c = (seq_t)x;
        seq_t new_num = S_num * c + S_den;
        seq_t new_den = S_den * c;
        seq_t g = gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;
        S_num = new_num;
        S_den = new_den;

        /* Increment the count of processed composites */
        ++comps_len;

        /* Compute a(comps_len) = denominator of the harmonic mean of the first comps_len composites */
        seq_t g2 = gcd((seq_t)comps_len * S_den, S_num);
        seq_t denom = S_num / g2;

        /* Ensure capacity in the results array */
        if (results_len >= results_cap) {
            size_t new_cap = results_cap ? results_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            results = tmp;
            results_cap = new_cap;
        }
        results[results_len++] = denom;
    }

    return results[n-1];
}
