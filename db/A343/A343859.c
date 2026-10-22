/*
 * A343859  Partial sums of the primes excluding 3.
 * https://oeis.org/A343859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of the primes excluding 3; equivalently
 *            a(n)=A007504(n+1)-3
 * Range:     u8 n <= 12, u16 n <= 156, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes partial sums of primes congruent to {1,2} mod 3 (i.e., all primes except 3). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static bool initialized = false;
static idx_t sieve_limit = 2000000;   /* initial sieve bound (large enough for many n) */
static bool *is_composite = NULL;
static seq_t *prefix = NULL;
static idx_t prefix_size = 0;

/* Helper: compute sieve up to 'limit' and fill prefix array with sums of primes !=3 */
static void compute_prefix(idx_t limit, seq_t **out_prefix, idx_t *out_count) {
    /* Allocate new composites array */
    bool *comp = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!comp) {
        /* In a real system, handle allocation error */
        return;
    }

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (!comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                comp[j] = true;
        }
    }

    /* Count primes (excluding 3) up to limit */
    idx_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i] && i != 3) ++cnt;

    /* Allocate prefix sums array */
    seq_t *pref = (seq_t *)malloc((size_t)cnt * sizeof(seq_t));
    if (!pref) {
        free(comp);
        return;
    }

    /* Compute prefix sums */
    seq_t sum = (seq_t)0;
    idx_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!comp[i] && i != 3) {
            sum += (seq_t)i;
            pref[pos++] = sum;
        }
    }

    /* Free old structures */
    free(is_composite);
    free(prefix);

    /* Update globals */
    is_composite = comp;
    prefix = pref;
    prefix_size = cnt;
    *out_prefix = pref;
    *out_count = cnt;
}

/* Ensure we have computed prefix sums up to at least 'needed_n' */
static void ensure_prefix(idx_t needed_n) {
    if (initialized && prefix_size >= needed_n)
        return;

    /* Recompute with increasing limit until enough primes are found */
    while (true) {
        compute_prefix(sieve_limit, &prefix, &prefix_size);
        if (prefix_size >= needed_n)
            break;
        sieve_limit *= 2;   /* double the bound and try again */
    }

    initialized = true;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, but return 0 safely */

    ensure_prefix(n);
    return prefix[n - 1];  /* a(1) is prefix[0] */
}
