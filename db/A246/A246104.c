/*
 * A246104  Least m > 0 for which (s(m), ..., s(n+m-1)) = (s(0), ..., s(n)),
 *          the first n+1 terms of the infinite Fibonacci word A003849.
 * https://oeis.org/A246104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 9344, u32 n <= 9344, u64 n <= 9344,
 *            u128 n <= 9344, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A246104:
   a(n) = least m > n+1 such that (s(m),…,s(m+n)) = (s(0),…,s(n)),
   where s(i) are the bits of the infinite Fibonacci word A003849.
   Implementation uses KMP to search the pattern in the infinite word.  */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Precomputed Fibonacci numbers up to 64‑bit limit for word generation. */
static idx_t fib_buf[90];
static size_t fib_cnt = 0;

/* Initialise fib_buf once. */
static void fib_init(void)
{
    if (fib_cnt != 0) return;
    fib_buf[0] = 0;
    fib_buf[1] = 1;
    fib_cnt = 2;
    while (fib_cnt < 90) {
        idx_t a = fib_buf[fib_cnt - 1];
        idx_t b = fib_buf[fib_cnt - 2];
        if (a > UINT64_MAX - b) break;
        fib_buf[fib_cnt] = a + b;
        ++fib_cnt;
    }
}

/* Return the i‑th character (0 or 1) of the infinite Fibonacci word A003849. */
static unsigned char fib_word_char(idx_t pos)
{
    fib_init();
    /* Find k such that |w_k| = F_{k+2} > pos.
       Note: fib_buf[k+2] is the length of w_k. */
    size_t k = 0;
    while (k + 2 < fib_cnt && fib_buf[k + 2] <= pos) ++k;

    idx_t p = pos;
    while (true) {
        if (k == 0) return 0;               /* w_0 = 0                    */
        if (k == 1) return (p == 1 ? 1 : 0); /* w_1 = 01                   */
        idx_t first_len = fib_buf[k + 1];    /* |w_{k-1}|                  */
        if (p < first_len) {
            --k;                            /* go to w_{k-1}              */
        } else {
            p -= first_len;
            k -= 2;                         /* go to w_{k-2}              */
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Pattern length L = n+1. */
    size_t L = (size_t)(n + 1);

    /* Allocate pattern array (0/1). */
    unsigned char *pat = (unsigned char *)malloc(L);
    if (!pat) return (seq_t)0;
    for (size_t i = 0; i < L; ++i) pat[i] = fib_word_char((idx_t)i);

    /* Build KMP prefix function. */
    int *pi = (int *)malloc(L * sizeof(int));
    if (!pi) { free(pat); return (seq_t)0; }
    pi[0] = 0;
    for (size_t i = 1; i < L; ++i) {
        int j = pi[i - 1];
        while (j > 0 && pat[i] != pat[j]) j = pi[j - 1];
        if (pat[i] == pat[j]) ++j;
        pi[i] = j;
    }

    /* Search the infinite word for the pattern.  */
    size_t i = 0;          /* text position, idx_t cast inside fib_word_char */
    int j = 0;             /* matched prefix length */
    while (true) {
        unsigned char c = fib_word_char((idx_t)i);
        while (j > 0 && c != pat[j]) j = pi[j - 1];
        if (c == pat[j]) ++j;
        if ((size_t)j == L) {
            size_t start = i - L + 1;   /* starting index of the match */
            if (start > (size_t)(n + 1)) {
                free(pat);
                free(pi);
                return (seq_t)start;   /* minimal m > n+1 found */
            }
            j = pi[j - 1];              /* continue searching */
        }
        ++i;
    }

    /* Unreachable but keeps compiler happy. */
    free(pat);
    free(pi);
    return (seq_t)0;
}
