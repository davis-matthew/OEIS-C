/*
 * A260936  Numbers m such that A260933(m) = m.
 * https://oeis.org/A260936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m with A260933(m) = m; computed by simulating the
 *            greedy involution of A260933 and collecting its fixed points
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9834 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static unsigned char *A260936_used = NULL;
static size_t A260936_cap = 0;
static idx_t *A260936_fixed = NULL;
static size_t A260936_fcnt = 0, A260936_fcap = 0;
static idx_t A260936_next = 1;

static void A260936_ensure(size_t need)
{
    if (A260936_cap > need)
        return;
    size_t nc = (A260936_cap == 0) ? 1024 : A260936_cap;
    while (nc <= need)
        nc <<= 1;
    unsigned char *p = (unsigned char *)realloc(A260936_used, nc);
    if (!p) abort();
    for (size_t i = A260936_cap; i < nc; ++i)
        p[i] = 0;
    A260936_used = p;
    A260936_cap = nc;
}

static void A260936_push(idx_t v)
{
    if (A260936_fcnt == A260936_fcap) {
        size_t nc = A260936_fcap ? A260936_fcap * 2 : 256;
        idx_t *p = (idx_t *)realloc(A260936_fixed, nc * sizeof(idx_t));
        if (!p) abort();
        A260936_fixed = p;
        A260936_fcap = nc;
    }
    A260936_fixed[A260936_fcnt++] = v;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    while (A260936_fcnt < (size_t)n) {
        idx_t i = A260936_next++;
        A260936_ensure((size_t)i);
        if (A260936_used[i])
            continue;
        for (idx_t j = i; ; ++j) {
            A260936_ensure((size_t)j);
            if (A260936_used[j])
                continue;
            if (!is_prime(i + j) && !is_prime(i + j + 1)) {
                A260936_used[i] = 1;
                A260936_used[j] = 1;
                if (j == i)
                    A260936_push(i);
                break;
            }
        }
    }
    return (seq_t)A260936_fixed[n - 1];
}
