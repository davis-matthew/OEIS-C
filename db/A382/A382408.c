/*
 * A382408  a(n) is the number of terms in A071174 whose radical is
 *          A144338(n).
 * https://oeis.org/A382408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(A144338(n) - 1, A144338(n) - Omega(A144338(n)))
 * Range:     u8 n <= 17, u16 n <= 127, u32 n <= 1403, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9840 (2026-11-10)
 * Helpers:   is_squarefree, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/mem.h"

static seq_t A144338_a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t count = 0;
    static size_t cap = 0;
    static idx_t last_checked = 1;

    while (count < n) {
        if (count == (idx_t)cap) {
            size_t new_cap = (cap == 0) ? 16 : cap << 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }
        ++last_checked;
        if (is_squarefree((seq_t)last_checked)) {
            cache[count++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}

seq_t a(idx_t n)
{
    seq_t s = A144338_a(n);
    /* s is squarefree, so Omega(s) = number of distinct primes */
    seq_t m = s;
    unsigned w = 0;
    if (m % 2 == 0) { w++; m /= 2; }
    for (seq_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            w++;
            m /= p;
        }
    }
    if (m > 1) w++;

    /* binomial(s-1, w-1), computed multiplicatively */
    seq_t top = s - 1;
    seq_t res = 1;
    for (unsigned i = 0; i + 1 < w; i++) {
        res = res * (top - (seq_t)i) / (seq_t)(i + 1);
    }
    return res;
}
