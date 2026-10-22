/*
 * A308275  Sum of the perimeters of the integer-sided triangles with
 *          perimeter n whose sides lengths are square numbers.
 * https://oeis.org/A308275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (# of integer‑sided triangles with side lengths *
 *            squares and perimeter n). The function counts all triples
 *            (a,b,c) * of positive square integers such that a ≤ b ≤ c, a + b
 *            + c = n, * and a + b > c, then returns n times that count
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External linkage for the required function. */
seq_t a(idx_t n)
{
    /* For n < 3 there is no triangle with positive sides. */
    if (n < 3) return (seq_t)0;

    /* Compute floor(sqrt(n)) using integer arithmetic. */
    idx_t max_root = 0;
    for (idx_t i = 1; i * i <= n; ++i) max_root = i;
    if (max_root == 0) return (seq_t)0;

    /* Store squares i^2 for i = 1 .. max_root. */
    idx_t *sqs = (idx_t *)malloc(max_root * sizeof(idx_t));
    if (!sqs) return (seq_t)0; /* unlikely for small n */
    for (idx_t i = 0; i < max_root; ++i)
        sqs[i] = (i + 1) * (i + 1);

    /* Boolean array to test if a number ≤ n is a square. */
    bool *is_square = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!is_square) { free(sqs); return (seq_t)0; }
    for (idx_t i = 0; i < max_root; ++i)
        is_square[(size_t)sqs[i]] = true;

    idx_t count = 0;
    for (idx_t ai = 0; ai < max_root; ++ai) {
        idx_t a = sqs[ai];
        /* a ≤ b ≤ c and a+b>c => a ≤ n/3 */
        if (a * 3 > n) break;

        for (idx_t bi = ai; bi < max_root; ++bi) {
            idx_t b = sqs[bi];
            idx_t sum_ab = a + b;
            if (sum_ab > n) break; /* c would be negative */

            idx_t c = n - sum_ab;
            if (c < b) break;      /* enforce a ≤ b ≤ c */

            if (!is_square[c]) continue;          /* c must be square */
            if (a + b <= c) continue;            /* triangle inequality */

            ++count;
        }
    }

    free(sqs);
    free(is_square);

    /* Compute n * count in seq_t arithmetic. */
    seq_t sn = (seq_t)n;
    seq_t sc = (seq_t)count;
    return sn * sc;
}
