/*
 * A285904  Partial row products of table A027746, prime factors with
 *          repetition, reversed.
 * https://oeis.org/A285904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 673, u16 n <= 9195, u32 n <= 9195, u64 n <= 9195,
 *            u128 n <= 9195, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute partial row products of table A027746 (prime factors
 * with repetition, reversed).  For an index n the algorithm first locates
 * the integer m such that the cumulative number of prime factors up to m
 * (with the first row counted as length 1 for m = 1) satisfies
 *   cum[m-1] < n ≤ cum[m].
 * The offset within that row is k = n - cum[m-1] - 1 (0‑based).  The
 * desired term is the product of the last k+1 prime factors of m,
 * arranged in ascending order (hence equivalent to the first k+1
 * elements of the reversed prime‑factor list).  The algorithm uses an
 * array of smallest prime factors (SPF) and a cumulative array of
 * prime‑factor counts (Ω).  Both arrays are grown as necessary. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static idx_t *spf = NULL;
static idx_t *cum = NULL;
static idx_t capacity = 0;
static bool initialized = false;

/* Recompute SPF and cumulative Ω arrays up to new_cap.  The function
 * reallocates the arrays and recomputes all values.  This is adequate
 * for the required problem size. */
static void ensure_capacity(idx_t new_cap)
{
    if (new_cap <= capacity)
        return;

    /* Determine the target size, at least 16. */
    if (new_cap < 16)
        new_cap = 16;
    else if (capacity > 0 && new_cap < capacity * 2)
        new_cap = capacity * 2;

    idx_t *new_spf = (idx_t *)realloc(spf, (new_cap + 1) * sizeof(idx_t));
    idx_t *new_cum = (idx_t *)realloc(cum, (new_cap + 1) * sizeof(idx_t));
    if (!new_spf || !new_cum)
        return; /* on failure we keep the old tables */

    spf = new_spf;
    cum = new_cum;
    capacity = new_cap;

    /* Compute SPF array (sieve of Eratosthenes). */
    for (idx_t i = 0; i <= capacity; ++i)
        spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;
    for (idx_t i = 2; i * i <= capacity; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            for (idx_t j = i * i; j <= capacity; j += i)
                if (spf[j] == 0)
                    spf[j] = i;
        }
    }
    for (idx_t i = 2; i <= capacity; ++i)
        if (spf[i] == 0)
            spf[i] = i;

    /* Compute cumulative Ω array, with row 1 having length 1. */
    cum[0] = 0;
    cum[1] = 1;
    for (idx_t i = 2; i <= capacity; ++i) {
        idx_t t = i;
        idx_t cnt = 0;
        while (t > 1) {
            t /= spf[t];
            ++cnt;
        }
        cum[i] = cum[i - 1] + cnt;
    }
    initialized = true;
}

/* Return the product of the last (offset+1) ascending prime factors
 * of m.  offset is 0‑based; for m == 1 the only term is 1. */
static seq_t partial_product(idx_t m, size_t offset)
{
    if (m == 1)
        return (seq_t)1;

    /* Collect ascending prime factors into a local stack. */
    idx_t factors[64];
    size_t nf = 0;
    idx_t t = m;
    while (t > 1) {
        idx_t p = spf[t];
        factors[nf++] = p;
        t /= p;
    }

    /* Multiply the last (offset+1) factors. */
    seq_t prod = (seq_t)1;
    size_t k = offset + 1;
    for (size_t i = 0; i < k; ++i)
        prod *= (seq_t)factors[nf - 1 - i];
    return prod;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Ensure tables are built to a reasonable starting size. */
    if (!initialized)
        ensure_capacity(16);

    /* Grow tables until we have cum[capacity] ≥ n. */
    while (cum[capacity] < n)
        ensure_capacity(capacity * 2);

    /* Binary search for minimal m with cum[m] ≥ n. */
    idx_t low = 1, high = capacity;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (cum[mid] >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Offset within the row (0‑based). */
    size_t offset = (size_t)(n - cum[m - 1] - 1);

    return partial_product(m, offset);
}
