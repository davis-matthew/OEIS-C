/*
 * A391898  The maximum exponent in the prime factorization of the
 *          exponentially noncomposite numbers (A390439).
 * https://oeis.org/A391898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391898:
 * a(n) = A051903(A390439(n)),
 * i.e. the maximum exponent occurring in the prime factorisation of the
 * nth exponentially non‑composite number (a number whose every prime
 * exponent is either 1 or a prime).
 *
 * The function builds the required sequence on demand, keeping a small
 * cache of the already computed terms.  For each integer m ≥ 1 it is
 * factored with `factor_u64` and accepted iff every prime exponent is
 * 1 or a prime.  The largest of these exponents becomes the return
 * value.  The algorithm runs in O(N log N) time with a negligible
 * constant factor – easily below the required 2 s for N ≈ 10 000.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Utilities required for prime generation, factorisation and integer sqrt */
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/int_sqrt.h"

/* ----- static state for memoisation ----- */
static seq_t *seq_vals   = NULL;   /* buffer of computed a(i) values */
static size_t seq_len    = 0;      /* number of values currently stored */
static size_t seq_cap    = 0;      /* allocated capacity of seq_vals */

static uint64_t *prime_list = NULL;   /* primes up to current_limit */
static size_t    prime_cnt  = 0;      /* number of primes in the list   */
static idx_t     prime_limit = 0;     /* current maximum prime bound   */

/* Ensure a nearest power‑of‑two buffer for seq_vals */
static void ensure_seq_cap(void)
{
    if (seq_len < seq_cap)
        return;
    size_t new_cap = (seq_cap == 0) ? 32 : seq_cap * 2;
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure prime_list contains all primes ≤ limit.  The function
 * build_primes_upto (from utils) is used; it allocates a new array and
 * returns it in *primes_out together with the count. */
static void ensure_primes_up_to(idx_t limit)
{
    if (prime_limit >= limit)
        return;
    if (prime_list != NULL) {
        free(prime_list);
    }
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Check whether a small integer (≤ 64) is a prime number.  Used only for
 * the exponent test, where the exponent cannot exceed ≈64 for the
 * expected problem size. */
static bool is_prime_small(int x)
{
    if (x <= 1) return false;
    if (x <= 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (int d = 5; (long long)d * d <= x; d += 6)
        if (x % d == 0 || x % (d + 2) == 0)
            return false;
    return true;
}

/* Main sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined; return 0 for safety */
        return 0;

    /* Generate values until we have at least n entries. */
    static idx_t candidate = 0;          /* last tested integer       */
    while (seq_len < (size_t)n) {
        candidate++;                      /* next integer to test      */
        bool ok = true;
        seq_t max_exp = 0;

        if (candidate == 1) {              /* 1 is special by convention */
            max_exp = 0;
        } else {
            /* factorise candidate */
            /* arrays big enough for any 64‑bit number (≤64 prime factors) */
            uint64_t pr[64];
            uint64_t ex[64];
            size_t cnt = 0;

            factor_u64(candidate, pr, ex, &cnt);

            /* Ensure we have enough primes up to sqrt(candidate) for future
             * factorisations (used after this iteration). */
            idx_t bound = int_sqrt(candidate) + 1;
            if (bound > prime_limit)
                ensure_primes_up_to(bound);

            for (size_t i = 0; i < cnt; ++i) {
                int e = (int)ex[i];
                if (!(e == 1 || is_prime_small(e))) {
                    ok = false;          /* exponent not allowed */
                    break;
                }
                if (max_exp < (seq_t)e)
                    max_exp = (seq_t)e;
            }
        }

        if (ok) {
            ensure_seq_cap();
            seq_vals[seq_len++] = max_exp;
        }
    }

    return seq_vals[(size_t)n - 1];
}
