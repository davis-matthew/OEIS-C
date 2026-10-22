/*
 * A355454  a(n) = 1 if the fourth smallest divisor of n is a square,
 *          otherwise 0.
 * https://oeis.org/A355454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 9450, u32 n <= 9450, u64 n <= 9450,
 *            u128 n <= 9450, big any n
 * Helpers:   floor_sqrt, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The sequence A355454 is defined by: 
 * a(n) = 1 if the fourth smallest divisor of n is a perfect square; 
 * a(n) = 0 otherwise. 
 * The function below computes this value for any n >= 1.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"     /* for int_sqrt(idx_t) */
#include "../utils/floor_sqrt.h"   /* for floor_sqrt(seq_t) */

/* Compare two idx_t values for qsort */
static int cmp_idx_t(const void *p, const void *q)
{
    const idx_t *a = (const idx_t *)p;
    const idx_t *b = (const idx_t *)q;
    return (*a > *b) - (*a < *b);
}

/* Compute the n-th term of A355454 */
seq_t a(idx_t n)
{
    /* Quick return for n with fewer than four divisors */
    if (n < 4) {
        return 0;
    }

    /* Array large enough for all divisors of n (n <= 10000 => tau(n) <= 128) */
    idx_t divs[128];
    size_t count = 0;

    /* Enumerate all divisors up to sqrt(n) */
    for (idx_t i = 1; i <= int_sqrt(n); ++i) {
        if (n % i == 0) {
            if (count < 128) {
                divs[count++] = i;
            }
            idx_t other = n / i;
            if (other != i) {
                if (count < 128) {
                    divs[count++] = other;
                }
            }
        }
    }

    /* If fewer than four divisors, the answer is 0 */
    if (count < 4) {
        return 0;
    }

    /* Sort the divisors to locate the fourth smallest */
    qsort(divs, count, sizeof(idx_t), cmp_idx_t);

    /* Fourth smallest divisor (0‑based index 3) */
    seq_t fourth = (seq_t)divs[3];

    /* Determine if it is a perfect square */
    seq_t root = floor_sqrt(fourth);
    if (root * root == fourth) {
        return 1;
    } else {
        return 0;
    }
}
