/*
 * A389108  Number of 4 X 4 matrices in Hermite normal form with determinant
 *          n.
 * https://oeis.org/A389108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{ d1*d2*d3*d4 = n ; d1 <= d2 <= d3 <= d4 } d2 * d3^2
 *            * d4^3
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

/* comparison routine for qsort on unsigned 64-bit integers */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* sequence starts at n = 1 */
    if (n == 1) return 1;

    /* --- factor n --- */
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* --- generate all divisors of n --- */
    idx_t *divs = NULL;
    size_t divcap = 8, divsize = 0;
    divs = (idx_t *)malloc(divcap * sizeof(idx_t));
    if (!divs) return 0;            /* allocation failure */
    divs[divsize++] = 1;            /* divisor 1 */

    for (size_t pi = 0; pi < pcnt; ++pi) {
        uint64_t p = primes[pi];
        idx_t e = exps[pi];
        idx_t p_pow = 1;
        size_t cur_size = divsize;  /* divisors so far */
        for (idx_t exp = 1; exp <= e; ++exp) {
            p_pow *= p;
            for (size_t i = 0; i < cur_size; ++i) {
                idx_t val = divs[i] * p_pow;
                if (divsize == divcap) {
                    divcap *= 2;
                    divs = (idx_t *)realloc(divs, divcap * sizeof(idx_t));
                    if (!divs) return 0; /* allocation failure */
                }
                divs[divsize++] = val;
            }
        }
    }

    /* sort divisors */
    qsort(divs, divsize, sizeof(idx_t), cmp_uint64);

    /* --- compute the sum --- */
    seq_t sum = 0;
    for (size_t ii = 0; ii < divsize; ++ii) {
        idx_t d2 = divs[ii];
        for (size_t jj = ii; jj < divsize; ++jj) {
            idx_t d3 = divs[jj];
            idx_t prod23 = d2 * d3;
            if (prod23 > n) break;
            for (size_t kk = jj; kk < divsize; ++kk) {
                idx_t d4 = divs[kk];
                idx_t prod234 = prod23 * d4;
                if (prod234 > n) break;
                if (n % prod234 != 0) continue;
                idx_t d1 = n / prod234;
                if (d1 <= d2) {
                    seq_t term = ((seq_t)d2) *
                                 seq_pow((seq_t)d3, 2) *
                                 seq_pow((seq_t)d4, 3);
                    sum += term;
                }
            }
        }
    }

    free(divs);
    return sum;
}
