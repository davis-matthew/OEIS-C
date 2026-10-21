/*
 * A243903  Numbers n such that (number of primes <= n) is greater than or
 *          equal to (number of semiprimes <= n).
 * https://oeis.org/A243903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m such that the count of primes ≤ m is
 *            greater than or equal to the count of semiprimes ≤ m
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for the sequence */
static seq_t *good = NULL;
static size_t good_cap = 0;
static size_t good_len = 0;

/* enlarge the storage when needed */
static void ensure_good_cap(size_t needed)
{
    if (good_cap >= needed) return;
    size_t new_cap = good_cap ? good_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(good, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    good = tmp;
    good_cap = new_cap;
}

/* initialise the sequence up to an arbitrary limit */
static void init_sequence(void)
{
    if (good_len != 0) return;          /* already initialised */

    /* limit large enough to contain all terms that will ever occur;
       the sequence is finite and the conjecture says no terms beyond
       the first few dozen.  200 000 is comfortably above that. */
    const idx_t LIMIT = 200000;

    /* sieve for primes and for smallest prime factor */
    uint8_t  *is_prime = (uint8_t *)calloc((size_t)LIMIT + 1, sizeof *is_prime);
    uint32_t *spf      = (uint32_t *)calloc((size_t)LIMIT + 1, sizeof *spf);
    if (!is_prime || !spf) abort();

    for (idx_t i = 2; i <= LIMIT; ++i) is_prime[i] = 1;

    for (idx_t i = 2; (idx_t)i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) {
                is_prime[j] = 0;
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    seq_t pi = 0, semi = 0;
    for (idx_t x = 1; x <= LIMIT; ++x) {
        if (x >= 2 && is_prime[x]) ++pi;

        if (x >= 2 && spf[x] != 0) {
            idx_t y = x / spf[x];
            if (y >= 2 && spf[y] == 0) ++semi;   /* semiprime */
        }

        if (pi >= semi) {
            ensure_good_cap(good_len + 1);
            good[good_len++] = (seq_t)x;
        }
    }

    free(is_prime);
    free(spf);
}

/* required function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* out of range – not expected */
    init_sequence();
    if ((size_t)n - 1 >= good_len) {
        /* The sequence is finite; if the requested index is larger
           than we have computed, return the last term found. */
        return good[good_len - 1];
    }
    return good[(size_t)n - 1];
}
