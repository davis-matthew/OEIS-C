/*
 * A337290  Numbers k such that both k and k+1 are in A020899.
 * https://oeis.org/A337290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that both k and k+1 have an odd number of
 *            terms in their Zeckendorf representation (A020899); computed by
 *            a memoized linear scan
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9586 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#define A337290_MAX_FIB 90
static idx_t A337290_fibs[A337290_MAX_FIB];
static size_t A337290_fib_len = 0;

static void A337290_ensure_fibs(idx_t limit)
{
    if (A337290_fib_len == 0) {
        A337290_fibs[0] = 1;
        A337290_fibs[1] = 2;
        A337290_fib_len = 2;
    }
    while (A337290_fib_len < A337290_MAX_FIB && A337290_fibs[A337290_fib_len - 1] <= limit) {
        A337290_fibs[A337290_fib_len] = A337290_fibs[A337290_fib_len - 1] + A337290_fibs[A337290_fib_len - 2];
        A337290_fib_len++;
    }
}

static bool A337290_odd(idx_t n)
{
    if (n == 0)
        return false;
    A337290_ensure_fibs(n);
    bool parity = false;
    size_t i = A337290_fib_len;
    while (n > 0) {
        while (i > 0 && A337290_fibs[i - 1] > n)
            --i;
        n -= A337290_fibs[i - 1];
        parity = !parity;
    }
    return parity;
}

static idx_t *A337290_terms = NULL;
static idx_t A337290_count = 0;
static idx_t A337290_cap = 0;
static idx_t A337290_next = 1;      /* next k to test */
static bool A337290_next_odd = true; /* parity of A337290_next (1 is odd) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    while (A337290_count < n) {
        idx_t k = A337290_next;
        bool ok = A337290_next_odd;
        bool ok1 = A337290_odd(k + 1);
        if (ok && ok1) {
            if (A337290_count == A337290_cap) {
                idx_t nc = A337290_cap ? A337290_cap * 2 : 1024;
                idx_t *p = (idx_t *)realloc(A337290_terms, (size_t)nc * sizeof(idx_t));
                if (!p)
                    return (seq_t)0;
                A337290_terms = p;
                A337290_cap = nc;
            }
            A337290_terms[A337290_count++] = k;
        }
        A337290_next = k + 1;
        A337290_next_odd = ok1;
    }
    return (seq_t)A337290_terms[n - 1];
}
