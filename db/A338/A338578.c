/*
 * A338578  Primes p such that (r-p)*(r-q) > r, where q and r are the next two
 *          primes.
 * https://oeis.org/A338578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   Prime p such that (r‑p)*(r‑q) > r, where q and r are the next
 *            two primes
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

static seq_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;


/* Grow a seq_t array to at least requested capacity. */
static void grow_array(seq_t **arr, size_t *len, size_t *cap, size_t req) {
    if (*cap >= req) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 8;
    while (new_cap < req) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);          /* Stop on allocation failure. */
    *arr = tmp;
    *cap = new_cap;
}

/* Ensure we have enough terms in the sequence to answer a query for n. */
static void ensure_sequence(idx_t n) {
    if (seq_len >= (size_t)n) return;          /* already enough */

    /* Initialise primality scan if not done already. */
    static bool init = false;
    static seq_t candidate = (seq_t)3;          /* candidate for next prime */

    if (!init) {
        grow_array(&primes, &primes_len, &primes_cap, 1);
        primes[0] = (seq_t)2;          /* the first prime */
        primes_len = 1;
        init = true;
    }

    while (seq_len < (size_t)n) {
        /* Find next prime. */
        while (!is_prime_big(candidate)) {
            candidate += (seq_t)2;
        }

        /* Record prime. */
        grow_array(&primes, &primes_len, &primes_cap, primes_len + 1);
        primes[primes_len++] = candidate;

        /* Evaluate condition when we have at least three primes. */
        if (primes_len >= 3) {
            seq_t p = primes[primes_len - 3];
            seq_t q = primes[primes_len - 2];
            seq_t r = primes[primes_len - 1];

            seq_t diff1 = r - p;
            seq_t diff2 = r - q;
            seq_t prod = diff1 * diff2;            /* safe in seq_t */

            if (prod > r) {
                grow_array(&seq, &seq_len, &seq_cap, seq_len + 1);
                seq[seq_len++] = p;
            }
        }

        /* Advance to next odd candidate. */
        candidate += (seq_t)2;
    }
}

/* External function required by the test harness. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;               /* undefined for index < 1 */
    ensure_sequence(n);
    return seq[n - 1];
}
