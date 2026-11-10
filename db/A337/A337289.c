/*
 * A337289  Numbers k such that k+1 is in A095096 and k is in A020899.
 * https://oeis.org/A337289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that k has an odd number of terms in its
 *            Zeckendorf representation and k+1 has an even number (found by a
 *            cached linear search)
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9608 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

#define A337289_MAXF 200
static seq_t A337289_fib[A337289_MAXF];
static int A337289_fc = 0;

static void A337289_init(void)
{
    if (A337289_fc) return;
    A337289_fib[0] = 1;
    A337289_fib[1] = 2;
    A337289_fc = 2;
    while (A337289_fc < A337289_MAXF) {
        seq_t nx = A337289_fib[A337289_fc - 1] + A337289_fib[A337289_fc - 2];
        if (nx < A337289_fib[A337289_fc - 1]) break; /* overflow */
        A337289_fib[A337289_fc++] = nx;
    }
}

/* true if number of Zeckendorf terms of n is odd */
static bool A337289_odd(seq_t n)
{
    A337289_init();
    bool p = false;
    int i = A337289_fc - 1;
    seq_t rest = n;
    while (rest > 0) {
        while (A337289_fib[i] > rest) --i;
        rest -= A337289_fib[i];
        p = !p;
    }
    return p;
}

static seq_t *A337289_cache = NULL;
static size_t A337289_sz = 0, A337289_cap = 0;
static seq_t A337289_next = 1;
static bool A337289_nextodd = true; /* parity of A337289_next */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A337289_sz < n) {
        seq_t k = A337289_next;
        bool ko = A337289_nextodd;
        bool k1o = A337289_odd(k + 1);
        if (ko && !k1o) {
            if (A337289_sz == A337289_cap) {
                A337289_cap = A337289_cap ? A337289_cap * 2 : 64;
                A337289_cache = mem_resize(seq_t, A337289_cache, A337289_cap);
            }
            A337289_cache[A337289_sz++] = k;
        }
        A337289_next = k + 1;
        A337289_nextodd = k1o;
    }
    return A337289_cache[n - 1];
}
