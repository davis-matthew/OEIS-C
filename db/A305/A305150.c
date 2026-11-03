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
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9938 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Recursive helper: count admissible factorizations of 'remain'.
   'start' is the index in 'divs' from which to try new factors.
   'selected' holds the factors already chosen (length selcnt).
   'cnt' accumulates the number of valid factorizations. */
static void rec_factor(uint64_t remain,
                       size_t start,
                       uint64_t *selected,
                       size_t selcnt,
                       const uint64_t *divs,
                       size_t dcnt,
                       seq_t *cnt)
{
    if (remain == 1) {
        ++(*cnt);              /* found a valid factorization */
        return;
    }

    for (size_t i = start; i < dcnt; ++i) {
        uint64_t d = divs[i];
        if (d > remain) break;
        if (remain % d != 0) continue;

        /* ensure pairwise indivisibility with already chosen factors */
        bool ok = true;
        for (size_t j = 0; j < selcnt; ++j) {
            if (d % selected[j] == 0 || selected[j] % d == 0) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;

        selected[selcnt] = d;
        rec_factor(remain / d, i + 1, selected, selcnt + 1,
                   divs, dcnt, cnt);
    }
}

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
