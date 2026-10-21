/*
 * A253015  Sequence of determinants of matrices based on the digits of
 *          nonnegative integers.
 * https://oeis.org/A253015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy, dumb
 * Formula:   a(n) = det(B) where B is the n×n matrix with B(i,i) equal to the
 *            i‑th decimal digit of n (from most significant), and for i<j,
 *            B(i,j)=abs(B(i,j-1)-B(i+1,j)), for i>j, B(i,j)=B(i-1,j)+B(i,j+1)
 * Range:     i8 n <= 149, i16 n <= 999, i32 n <= 999, i64 n <= 999,
 *            i128 n <= 999, big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* count decimal digits of a uint64_t (header provided by the utils) */
#include "../utils/digit_count.h"

/* absolute value for seq_t (handles signed integer or big‑int types) */
static seq_t abs_seq(seq_t v)
{
    return v < 0 ? -v : v;
}

/* Main function computing A253015(n) */
seq_t a(idx_t n)
{
    /* Convert n to digits in reverse (least significant first) */
    uint8_t rev[20];      /* 20 digits is enough for uint64_t */
    size_t k = 0;
    uint64_t tmp = (uint64_t)n;
    do {
        rev[k++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    } while (tmp);

    /* Allocate matrix of size k × k dynamically (k ≤ 20) */
    size_t sz = k * k;
    seq_t *B = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!B) return (seq_t)0;  /* return 0 on allocation failure */

    /* Initialize all entries to 0 */
    for (size_t idx = 0; idx < sz; ++idx) B[idx] = (seq_t)0;

    /* Set diagonal entries: most significant digit first (index 0) */
    for (size_t i = 0; i < k; ++i) {
        B[i * k + i] = (seq_t)rev[k - 1 - i];
    }

    /* Fill upper triangle (i < j) by increasing distance */
    for (size_t delta = 1; delta < k; ++delta) {
        for (size_t i = 0; i + delta < k; ++i) {
            size_t j = i + delta;
            seq_t left  = B[i * k + (j - 1)];
            seq_t down  = B[(i + 1) * k + j];
            B[i * k + j] = abs_seq(left - down);
        }
    }

    /* Fill lower triangle (i > j) by increasing distance */
    for (size_t delta = 1; delta < k; ++delta) {
        for (size_t i = delta; i < k; ++i) {
            size_t j = i - delta;
            seq_t up    = B[(i - 1) * k + j];
            seq_t right = B[i * k + (j + 1)];
            B[i * k + j] = up + right;
        }
    }

    /* Compute determinant using the Bareiss (fraction‑free) algorithm */
    if (k == 1) {
        seq_t result = B[0];
        free(B);
        return result;
    }

    seq_t prev = (seq_t)1;  /* B(-1,-1) for first step */

    for (size_t r = 0; r < k - 1; ++r) {
        seq_t pivot = B[r * k + r];
        if (pivot == (seq_t)0) {
            free(B);
            return (seq_t)0;   /* singular matrix */
        }
        for (size_t i = r + 1; i < k; ++i) {
            for (size_t j = r + 1; j < k; ++j) {
                seq_t num = (B[i * k + j] * pivot)
                            - (B[i * k + r] * B[r * k + j]);
                B[i * k + j] = num / prev;
            }
        }
        prev = pivot;
    }

    seq_t result = B[(k - 1) * k + (k - 1)];
    free(B);
    return result;
}
