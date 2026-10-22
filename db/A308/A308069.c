/*
 * A308069  Number of integer-sided triangles with perimeter n whose side
 *          lengths are semiprime.
 * https://oeis.org/A308069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Number of integer‐sided triangles with perimeter n
   whose side lengths are all semiprime. The implementation enumerates
   all semiprime side lengths up to n and counts unordered triples
   (a,b,c) with a ≤ b ≤ c, a+b+c = n and a+b>c. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* A quick shortcut for very small n.  The loops below would
       return 0 in those cases, but this avoids allocating memory. */
    if (n < 12) {
        return (seq_t)0;
    }

    size_t limit = (size_t)n;

    /* Allocate array marking the presence of semiprime numbers. */
    bool *is_semiprime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_semiprime) {
        return (seq_t)0;
    }

    /* Build a list of primes up to limit. */
    idx_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes((idx_t)limit, &primes, &primes_cnt);

    /* Mark all products p*q (with p ≤ q) that are ≤ limit. */
    for (size_t ip = 0; ip < primes_cnt; ++ip) {
        idx_t p = primes[ip];
        for (size_t jq = ip; jq < primes_cnt; ++jq) {
            idx_t q = primes[jq];
            if (q > limit / p) {          /* avoid overflow and stop early */
                break;
            }
            idx_t prod = p * q;
            is_semiprime[(size_t)prod] = true;
        }
    }

    /* Build a compact list of semiprime values for faster iteration. */
    size_t list_cap = 256;
    size_t list_len = 0;
    idx_t *list = (idx_t *)malloc(list_cap * sizeof(idx_t));
    if (!list) {
        free(primes);
        free(is_semiprime);
        return (seq_t)0;
    }

    for (idx_t val = 4; val <= (idx_t)limit; ++val) {  /* 4 = first semiprime */
        if (is_semiprime[(size_t)val]) {
            if (list_len == list_cap) {
                list_cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(list, list_cap * sizeof(idx_t));
                if (!tmp) {
                    free(list);
                    free(primes);
                    free(is_semiprime);
                    return (seq_t)0;
                }
                list = tmp;
            }
            list[list_len++] = val;
        }
    }

    free(primes);

    /* Count triangles with sides a ≤ b ≤ c, all semiprime. */
    seq_t count = (seq_t)0;

    for (size_t i = 0; i < list_len; ++i) {
        idx_t a_val = list[i];
        if (a_val * 3 > n) {          /* a cannot exceed n/3 */
            break;
        }
        for (size_t j = i; j < list_len; ++j) {
            idx_t b_val = list[j];
            idx_t sum_ab = a_val + b_val;
            if (sum_ab > n) {         /* further b will only increase sum_ab */
                break;
            }
            idx_t c_val = n - sum_ab;
            if (c_val < b_val) {      /* ensure b ≤ c */
                continue;
            }
            if (sum_ab <= c_val) {    /* triangle inequality fails */
                continue;
            }
            if (!is_semiprime[(size_t)c_val]) {
                continue;
            }
            ++count;                  /* a valid triangle found */
        }
    }

    free(list);
    free(is_semiprime);
    return count;
}
