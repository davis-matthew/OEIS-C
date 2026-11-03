/*
 * A278049  a(n) = 3*(Sum_{k=1..n} phi(k)) - 1, where phi = A000010.
 * https://oeis.org/A278049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * (Sum_{k=1..n} φ(k)) - 1
 * Range:     u8 n <= 16, u16 n <= 267, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9033 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* φ(k) is obtained from the provided helper `phi(idx_t)` */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/totient.h"
#include "../utils/mem.h"

static seq_t *cumsum = NULL;      /* cumsum[i] = Σ_{k=1..i} φ(k) */
static idx_t capacity = 0;        /* allocated array size */
static idx_t max_n    = 0;        /* highest n fully computed */

static void
ensure_capacity(idx_t n)
{
    /* we need indices up to n, so allocate at least n+1 slots */
    if (n + 1 <= capacity)
        return;

    idx_t new_cap = capacity == 0 ? 16 : capacity * 2;
    while (new_cap <= n)
        new_cap *= 2;

    seq_t *tmp = mem_resize(seq_t, cumsum, (size_t)new_cap);
    if (!tmp)
        abort();                      /* allocation failed – terminate */

    cumsum   = tmp;
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n=0, return 0 */

    ensure_capacity(n);

    /* Compute missing values up to n */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        cumsum[k] = cumsum[k - 1] + totient(k);
    }
    max_n = n;

    seq_t sum_phi = cumsum[n];
    seq_t result  = sum_phi * (seq_t)3;
    result -= (seq_t)1;
    return result;
}
