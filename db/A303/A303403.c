/*
 * A303403  Even numbers that are not the sum of two prime-indexed primes.
 * https://oeis.org/A303403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function generates the OEIS sequence A303403. It repeatedly
 *            sieves primes up to a growing limit, builds the list of
 *            prime‑indexed primes, marks all even sums of two of them, and
 *            collects the remaining even numbers
 * Range:     u8 n <= 46, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   make_prime_flag, seq_alloc, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function combines the required helper utilities straight into the
   implementation; only the standard headers and the seq_alloc helpers
   are used. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_free_array.h"
#include "../utils/make_prime_flag.h"

/* Recompute the entire sequence for primes up to 'limit'.
   The resulting sequence is stored in *seq_ptr, its length in *len_ptr,
   and the limit used in *limit_ptr. */
static void _recompute_seq(idx_t limit, seq_t **seq_ptr,
                           size_t *len_ptr, idx_t *limit_ptr)
{
    /* --- Prime sieve ------------------------------------ */
    bool *is_prime_arr = make_prime_flag(limit);
    if (!is_prime_arr) return;

    /* Count primes up to limit */
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime_arr[i]) ++primes_cnt;

    /* Store the primes in an array */
    uint32_t *primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) { free(is_prime_arr); return; }
    size_t pidx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime_arr[i]) primes[pidx++] = (uint32_t)i;

    /* --- Prime‑indexed primes ---------------------------- */
    uint32_t *pi_primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    size_t pi_cnt = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        /* indices are 1‑based: prime at position i+1 */
        if (is_prime_arr[i + 1])
            pi_primes[pi_cnt++] = primes[i];
    }

    /* No longer need the full prime list */
    free(primes);

    /* --- Mark representable even sums ------------------- */
    bool *rep = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!rep) { free(is_prime_arr); free(pi_primes); return; }

    for (size_t i = 0; i < pi_cnt; ++i) {
        for (size_t j = i; j < pi_cnt; ++j) {
            idx_t sum = (idx_t)pi_primes[i] + (idx_t)pi_primes[j];
            if (sum <= limit) rep[sum] = true;
        }
    }

    /* --- Collect evens that are not representable -------- */
    seq_t *new_seq = seq_alloc(limit / 2 + 1);
    size_t new_len = 0;
    for (idx_t e = 2; e <= limit; e += 2) {
        if (!rep[e]) new_seq[new_len++] = (seq_t)e;
    }

    /* Clean up temporary arrays */
    free(is_prime_arr);
    free(pi_primes);
    free(rep);

    /* Return the new data */
    *seq_ptr   = new_seq;
    *len_ptr   = new_len;
    *limit_ptr = limit;
}

/* Public sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1 */

    /* Persistent state */
    static seq_t *seq  = NULL;
    static size_t seq_len = 0;
    static idx_t current_limit = 0;

    /* Initial build with a moderate limit */
    if (seq == NULL) {
        _recompute_seq(500000, &seq, &seq_len, &current_limit);
    }

    /* Grow the sequence until it contains at least n terms */
    while (seq_len < (size_t)n) {
        /* Double the limit (or start with 1 000 000) */
        idx_t new_limit = current_limit ? current_limit * 2 : 1000000;

        seq_t *old_seq   = seq;
        size_t old_len   = seq_len;
        idx_t old_limit  = current_limit;

        _recompute_seq(new_limit, &seq, &seq_len, &current_limit);

        /* Free the old array */
        seq_free_array(old_seq);
    }

    /* Return the requested term (0‑based indexing into seq) */
    return seq[n - 1];
}
