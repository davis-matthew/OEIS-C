/*
 * A392970  a(n) = prime(n)^(prime(n+prime(n)-1) - prime(n) + 1) *
 *          Product_{k=1..prime(n)-1} prime(k+n).
 * https://oeis.org/A392970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A392970: a(n) = prime(n)^(prime(n+prime(n)-1)-prime(n)+1)
 *            * ∏_{k=1}^{prime(n)-1} prime(k+n)  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility prototypes provided by the build system. */
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

/* Static storage for the prime table.  primes[i] is the (i+1)-th prime. */
static uint64_t *primes = NULL;       /* array of primes */
static size_t   prime_count = 0;      /* number of primes currently stored */
static idx_t    prime_limit = 100;    /* current upper bound used to generate primes */

/* Ensure that at least idx primes are present in the static table. */
static void
ensure_primes_up_to_index(idx_t idx)
{
    if (prime_count >= (size_t)idx)
        return;

    idx_t limit = prime_limit;
    while (1) {
        uint64_t *tmp = NULL;
        size_t    cnt = 0;
        build_primes_upto(limit, &tmp, &cnt);
        if (!tmp) {
            /* In case the library fails, abort to avoid endless loop. */
            abort();
        }
        if (cnt >= (size_t)idx) {
            free(primes);
            primes    = tmp;
            prime_count = cnt;
            prime_limit = limit;
            return;
        }
        free(tmp);
        limit *= 2;
    }
}

/* Main sequence function.  External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n=0; placeholder */

    /* Ensure we have prime(n). */
    ensure_primes_up_to_index(n);
    uint64_t p_n = primes[(size_t)n - 1];

    /* Index for the exponent prime: n + p_n - 1. */
    idx_t target_idx = n + (idx_t)p_n - 1;
    ensure_primes_up_to_index(target_idx);
    uint64_t p_target = primes[(size_t)target_idx - 1];

    /* Exponent: prime(target_idx) - prime(n) + 1. */
    seq_t exp = (seq_t)(p_target - p_n + 1);

    /* Base^{exp}. */
    seq_t pow_part = big_pow((seq_t)p_n, exp);

    /* Product of primes from index n+1 to target_idx. */
    seq_t prod = 1;
    for (idx_t i = n + 1; i <= target_idx; ++i)
        prod *= (seq_t)primes[(size_t)i - 1];

    /* Final result. */
    return pow_part * prod;
}
