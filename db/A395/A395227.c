/*
 * A395227  a(n) = number of 4-tuples (w, x, y, z) such that w*x + y*z = n,
 *          where w, x, y, z are positive Fibonacci numbers satisfying w < x
 *          and y < z.
 * https://oeis.org/A395227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts 4‑tuples (w,x,y,z) of positive Fibonacci numbers
 *            with w < x, y < z and w*x + y*z = n
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* No tuples for negative or zero n. */
    if (n == (idx_t)0) return (seq_t)0;

    /* ----- generate all Fibonacci values <= n (including duplicates). ----- */
    size_t fib_cap = 8;
    size_t fib_len = 0;
    idx_t *fibs = (idx_t *)malloc(fib_cap * sizeof(idx_t));
    if (!fibs) return (seq_t)0;          /* Out of memory – return 0. */

    idx_t a = 1, b = 1;
    while (a <= n) {
        if (fib_len >= fib_cap) {        /* grow the array */
            fib_cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(fibs, fib_cap * sizeof(idx_t));
            if (!tmp) {
                free(fibs); return (seq_t)0;
            }
            fibs = tmp;
        }
        fibs[fib_len++] = a;
        idx_t next = a + b;
        a = b;
        b = next;
    }

    /* ----- keep only unique Fibonacci values (sorted ascending). ----- */
    size_t uniq_len = 0;
    idx_t *uniq = (idx_t *)malloc(fib_len * sizeof(idx_t));
    if (!uniq) { free(fibs); return (seq_t)0; }

    for (size_t i = 0; i < fib_len; ++i) {
        if (uniq_len == 0 || fibs[i] != uniq[uniq_len - 1]) {
            uniq[uniq_len++] = fibs[i];
        }
    }
    free(fibs);

    /* ----- prepare an array to count products y*z for y<z. ----- */
    size_t size_arr = (size_t)n + 1;      /* indices 0..n                      */
    seq_t *prodCount = (seq_t *)calloc(size_arr, sizeof(seq_t));
    if (!prodCount) { free(uniq); return (seq_t)0; }

    for (size_t i = 0; i < uniq_len; ++i) {
        for (size_t j = i + 1; j < uniq_len; ++j) {
            idx_t prod = uniq[i] * uniq[j];
            if (prod > n) break;          /* products grow with j */
            prodCount[(size_t)prod] += (seq_t)1;
        }
    }

    /* ----- count quadruples by matching complementary products ----- */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < uniq_len; ++i) {
        for (size_t j = i + 1; j < uniq_len; ++j) {
            idx_t prod = uniq[i] * uniq[j];
            if (prod > n) break;
            idx_t complement = n - prod;
            result += prodCount[(size_t)complement];
        }
    }

    free(prodCount);
    free(uniq);
    return result;
}
