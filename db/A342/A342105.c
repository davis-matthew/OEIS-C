/*
 * A342105  Arithmetic numbers (A003601) that are not balanced numbers
 *          (A020492).
 * https://oeis.org/A342105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9457 (2026-11-03)
 * Helpers:   mem, num_divisors, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342105: Arithmetic numbers (A003601) that are not balanced
 * numbers (A020492).  A term m satisfies τ(m) | σ(m) and φ(m) ∤ σ(m). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/num_divisors.h"
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    /* Static cache of previously found terms */
    static seq_t *cache = NULL;          /* array of terms */
    static size_t cap = 0;               /* current capacity of cache */
    static size_t len = 0;               /* number of stored terms */
    static uint64_t last_checked = 1;    /* last integer examined */

    /* Ensure the caller's request can be satisfied */
    if (n <= len)
        return cache[n - 1];

    /* Grow the cache array lazily */
    if (cap == 0) {
        cap = 128;
        cache = mem_new(seq_t, cap);
    }

    /* Scan integers until the cache contains at least n terms */
    while (len < n) {
        ++last_checked;

        uint64_t tau = num_divisors(last_checked);
        seq_t sigma = sigma_of(last_checked);
        seq_t phi_val = totient_seq((seq_t)last_checked);

        seq_t tau_seq = (seq_t)tau;

        /* Check the defining conditions */
        if ((sigma % tau_seq == 0) && (sigma % phi_val != 0)) {
            /* Store the term */
            if (len == cap) {
                cap <<= 1;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
