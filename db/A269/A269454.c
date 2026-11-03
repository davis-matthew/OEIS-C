/*
 * A269454  Safe primes that are not congruent to -1 mod 8.
 * https://oeis.org/A269454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 240, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9715 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Safe primes that are not congruent to -1 mod 8
   a(n) = the nth safe prime p such that p % 8 != 7. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------ */
static void
generate_safe_primes(uint64_t limit, uint64_t **arr, size_t *cnt)
{
    if (limit < 2) {
        *arr   = NULL;
        *cnt   = 0;
        return;
    }

    /* Allocate and initialise a simple sieve */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (is_prime == NULL) {
        *arr = NULL;
        *cnt = 0;
        return;
    }
    memset(is_prime, 1, (size_t)(limit + 1) * sizeof(bool));
    is_prime[0] = is_prime[1] = false;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!is_prime[i])
            continue;
        for (uint64_t j = i * i; j <= limit; j += i)
            is_prime[j] = false;
    }

    /* Count safe primes, first pass to allocate storage */
    size_t tmp_cnt = 0;
    for (uint64_t p = 2; p <= limit; ++p) {
        if (!is_prime[p])
            continue;
        if ((p & 7) == 7)          /* p % 8 == 7 */
            continue;
        uint64_t q = (p - 1) >> 1; /* (p-1)/2 */
        if (q >= 2 && q <= limit && is_prime[q])
            ++tmp_cnt;
    }

    uint64_t *tmp_arr = (uint64_t *)malloc(tmp_cnt * sizeof(uint64_t));
    if (tmp_arr == NULL) {
        free(is_prime);
        *arr = NULL;
        *cnt = 0;
        return;
    }

    size_t idx = 0;
    for (uint64_t p = 2; p <= limit; ++p) {
        if (!is_prime[p])
            continue;
        if ((p & 7) == 7)
            continue;
        uint64_t q = (p - 1) >> 1;
        if (q >= 2 && q <= limit && is_prime[q]) {
            tmp_arr[idx++] = p;
        }
    }

    free(is_prime);
    *arr = tmp_arr;
    *cnt = tmp_cnt;
}

/* ------------------------------------------------------------------ */
/*  Static data                                               */
/* ------------------------------------------------------------------ */
static uint64_t *safe_primes_arr = NULL;   /* array of uint64_t primes */
static size_t safe_count = 0;              /* number of primes stored */

/* ------------------------------------------------------------------ */
/*  Helper: generate safe primes up to a given limit                  */

/* ------------------------------------------------------------------ */
/*  Helper: extend the stored safe-prime list until we have at least n*/
/* ------------------------------------------------------------------ */
static void
ensure_safe_primes(idx_t n)
{
    if (safe_count >= (size_t)n)
        return;

    /* Estimate an upper bound for the nth safe prime.
       The nth safe prime ≤ 10 * n for all n >= 1 (roughly).
       We start with a small limit and double until we have enough. */
    uint64_t limit = (uint64_t)n * 10 + 100;
    uint64_t *new_arr = NULL;
    size_t new_cnt   = 0;

    while (1) {
        generate_safe_primes(limit, &new_arr, &new_cnt);
        if (new_arr == NULL) {            /* allocation failure - abort */
            return;
        }
        if (new_cnt >= (size_t)n)
            break;
        free(new_arr);                    /* not enough, widen the interval */
        limit *= 2;
    }

    /* Replace the old array with the new one */
    free(safe_primes_arr);
    safe_primes_arr = new_arr;
    safe_count      = new_cnt;
}

/* ------------------------------------------------------------------ */
/*  Public entry point                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* undefined for n < 1 */
    ensure_safe_primes(n);
    if (n - 1 < safe_count)
        return (seq_t)safe_primes_arr[n - 1];
    return (seq_t)0;                        /* safety fallback */
}
