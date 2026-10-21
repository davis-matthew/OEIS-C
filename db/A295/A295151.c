/*
 * A295151  Numbers that have exactly three representations as a sum of five
 *          nonnegative squares.
 * https://oeis.org/A295151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers with exactly three representations as a sum of five
   nonnegative squares.  Computed by enumerating unordered 5‑square
   representations up to 5408. */
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define MAX_N 5408

static void compute_terms(seq_t *terms, size_t *term_count)
{
    static unsigned short count[MAX_N + 1];
    memset(count, 0, sizeof(count));

    /* Pre‑compute squares up to MAX_N */
    size_t sqrt_max = 0;
    while (sqrt_max * sqrt_max <= MAX_N) ++sqrt_max;
    --sqrt_max;                     /* sqrt_max is floor(sqrt(MAX_N)) */
    size_t squares[74];              /* 0 .. 73 */
    size_t sqcnt = 0;
    for (size_t s = 0; s <= sqrt_max; ++s)
        squares[sqcnt++] = s * s;

    /* Enumerate unordered 5‑tuples (a≤b≤c≤d≤e) of squares */
    for (size_t i = 0; i < sqcnt; ++i) {
        idx_t a = squares[i];
        for (size_t j = i; j < sqcnt; ++j) {
            idx_t sum1 = a + squares[j];
            if (sum1 > MAX_N) break;
            for (size_t k = j; k < sqcnt; ++k) {
                idx_t sum2 = sum1 + squares[k];
                if (sum2 > MAX_N) break;
                for (size_t l = k; l < sqcnt; ++l) {
                    idx_t sum3 = sum2 + squares[l];
                    if (sum3 > MAX_N) break;
                    for (size_t m = l; m < sqcnt; ++m) {
                        idx_t total = sum3 + squares[m];
                        if (total > MAX_N) break;
                        ++count[total];
                    }
                }
            }
        }
    }

    /* Collect numbers with exactly three representations */
    size_t tc = 0;
    for (idx_t n = 1; n <= MAX_N; ++n) {
        if (count[n] == 3) {
            terms[tc++] = (seq_t)n;
        }
    }
    *term_count = tc;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t terms[MAX_N + 1];
    static size_t term_count = 0;

    if (!initialized) {
        compute_terms(terms, &term_count);
        initialized = true;
    }

    if (n == 0 || n > term_count) return (seq_t)0;
    return terms[n - 1];
}
