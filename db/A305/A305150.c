/*
 * A305150  Number of factorizations of n into distinct, pairwise indivisible
 *          factors greater than 1.
 * https://oeis.org/A305150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of factorizations of n into distinct, pairwise
 *            indivisible factors greater than 1. The implementation
 *            enumerates all admissible factor lists using recursion and
 *            builds a list of divisors of n. It is valid C11 and C++17, uses
 *            only integer arithmetic, and works for arbitrary sized seq...
 * Range:     u8 n <= 9938, u16 n <= 9938, u32 n <= 9938, u64 n <= 9938,
 *            u128 n <= 9938, big any n
 * Helpers:   rec_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/rec_factor.h"

/* Comparator for qsort when sorting uint64_t values */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* by definition */

    /* Build list of divisors of n greater than 1. */
    uint64_t nn = (uint64_t)n;
    size_t dcap = 8;
    size_t dcnt = 0;
    uint64_t *divs = (uint64_t *)malloc(dcap * sizeof(uint64_t));
    if (!divs) return (seq_t)0;    /* allocation failure */

    for (uint64_t i = 2; i * i <= nn; ++i) {
        if (nn % i == 0) {
            if (dcnt == dcap) {
                dcap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(divs, dcap * sizeof(uint64_t));
                if (!tmp) { free(divs); return (seq_t)0; }
                divs = tmp;
            }
            divs[dcnt++] = i;
            uint64_t j = nn / i;
            if (j != i) {
                if (dcnt == dcap) {
                    dcap *= 2;
                    uint64_t *tmp = (uint64_t *)realloc(divs, dcap * sizeof(uint64_t));
                    if (!tmp) { free(divs); return (seq_t)0; }
                    divs = tmp;
                }
                divs[dcnt++] = j;
            }
        }
    }

    /* If n itself is not prime, add it as a divisor (always valid). */
    if (dcnt == dcap) {
        dcap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(divs, dcap * sizeof(uint64_t));
        if (!tmp) { free(divs); return (seq_t)0; }
        divs = tmp;
    }
    divs[dcnt++] = nn;

    /* Sort the divisors in ascending order */
    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    /* Run recursion to count valid factorizations */
    seq_t count = 0;
    uint64_t selected[64];   /* enough for n <= 10^4 */
    rec_factor(nn, 0, selected, 0, divs, dcnt, &count);

    free(divs);
    return count;
}
