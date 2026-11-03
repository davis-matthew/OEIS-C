/*
 * A351398  Numbers k >= 3 such that the arithmetic mean of the divisors of k
 *          AND the arithmetic mean of the nondivisors of k are integers.
 * https://oeis.org/A351398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k ≥ 3 such that the arithmetic mean of
 *            the divisors of k and the arithmetic mean of the non‑divisors of
 *            k are both integers. We generate the sequence by testing each
 *            integer k ≥ 3: if k is prime the condition holds automatically
 *            (for p ≥ 3), otherwise we compute the divis...
 * Range:     u8 n <= 25, u16 n <= 465, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9805 (2026-11-03)
 * Helpers:   is_prime, mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/num_divisors.h"
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Static cache for the generated sequence values */
static seq_t *seq_cache = NULL;
static size_t seq_size = 0, seq_cap = 0;

/* Last integer tested; starts just below the first candidate (3) */
static idx_t last_tested = 2;

/* Helper: test whether k satisfies the OEIS condition */
static bool k_satisfies(idx_t k)
{
    /* Prime numbers p >= 3 always satisfy the condition */
    if (is_prime(k))
        return true;

    /* For composites, compute σ(k) and τ(k) */
    uint64_t tau = num_divisors(k);          /* τ(k) */
    seq_t sigma = sigma_of(k);                    /* σ(k) */

    /* Integer arithmetic on seq_t */
    seq_t total = ((seq_t)k) * (((seq_t)k) + 1) / 2;          /* Σ i = k(k+1)/2 */
    seq_t sum_non = total - sigma;                           /* Σ non‑divisors */
    seq_t count_non = ((seq_t)k) - ((seq_t)tau);             /* non‑divisor count */

    /* Check divisibility of the two means */
    return (sigma % ((seq_t)tau) == 0) && (sum_non % count_non == 0);
}

/* Main function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n=0, but caller will avoid it */
        return 0;

    /* Extend the cache until we have at least n values */
    while (seq_size < n)
    {
        idx_t k = last_tested + 1;
        last_tested = k;

        if (k_satisfies(k))
            append_candidate(&seq_cache, &seq_size, &seq_cap, (seq_t)k);
    }

    return seq_cache[n - 1];
}
