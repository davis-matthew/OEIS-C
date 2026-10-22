/*
 * A317597  a(n) is the smallest even number for which there are n prime
 *          numbers between a(n) and the largest prime number p such that
 *          a(n)-p is also a prime.
 * https://oeis.org/A317597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A317597: a(n) is the smallest even number E such that
 * there are exactly n primes p with p < E and (E‑p) also prime,
 * where p is the largest such prime.
 *
 * The algorithm scans even numbers in increasing order, keeps a
 * growing list of primes up to the current even number and, for
 * each even E, finds the largest qualifying prime p.  The count
 * of primes strictly between p and E is then
 *      cnt = π(E‑1) – π(p).
 * The first E that yields a new count is stored as a[count].
 * Results are memoised so that arbitrary‑order calls are fast.
 *
 * A small double‑sized sieve strategy is used so that rebuilding
 * the prime list is infrequent.  All arithmetic on indices is
 * performed in idx_t, while sequence values are kept in seq_t.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_small.h"

/* --- Persistent state --------------------------------------------------- */
static uint64_t *prime_list = NULL;   /* primes ≤ prime_limit   */
static size_t   prime_cnt   = 0;       /* number of primes       */
static uint64_t prime_limit = 0;      /* current sieve limit    */

static seq_t  *a_values   = NULL;     /* a[n] values            */
static bool   *a_mask     = NULL;     /* a[n] known flag        */
static size_t  a_cap      = 0;         /* capacity of a_ arrays  */

static uint64_t next_even     = 4;     /* next even to examine   */
static bool      initialized  = false;

/* --- Utility: extend prime sieve --------------------------------------- */
static void extend_prime_sieve(uint64_t limit)
{
    if (limit <= prime_limit) return;

    /* Start from at least 200000 and double until we exceed limit. */
    uint64_t new_limit = prime_limit ? prime_limit : 200000;
    while (new_limit < limit) new_limit *= 2U;

    /* Rebuild prime list.  The old allocation is freed automatically. */
    if (prime_list) free(prime_list);
    build_primes_upto(new_limit, &prime_list, &prime_cnt);
    prime_limit = new_limit;
}

/* --- Utility: grow a_ arrays ------------------------------------------- */
static void grow_a_arrays(size_t new_cap)
{
    if (new_cap <= a_cap) return;
    seq_t *new_vals = (seq_t *)realloc(a_values, new_cap * sizeof(seq_t));
    bool  *new_mask = (bool  *)realloc(a_mask,   new_cap * sizeof(bool));

    /* Initialise any new slots. */
    for (size_t i = a_cap; i < new_cap; ++i) {
        new_vals[i] = (seq_t)0;
        new_mask[i] = false;
    }

    a_values = new_vals;
    a_mask   = new_mask;
    a_cap    = new_cap;
}

/* --- Main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise once. */
    if (!initialized) {
        extend_prime_sieve(200000);   /* base sieve size */
        grow_a_arrays(0);
        initialized = true;
    }

    size_t target = (size_t)n;
    if (target < a_cap && a_mask[target]) return a_values[target];

    /* Scan even numbers until the desired count is found. */
    while (true) {
        /* Ensure we have all primes up to the current even number. */
        extend_prime_sieve(next_even);

        /* Number of primes ≤ next_even‑1 : π(next_even‑1). */
        size_t primes_upto = 0;
        {
            size_t low = 0, high = prime_cnt;
            while (low < high) {
                size_t mid = (low + high) / 2;
                if (prime_list[mid] <= next_even - 1) low = mid + 1;
                else                               high = mid;
            }
            primes_upto = low;                   /* π(next_even‑1) */
        }

        /* Search descending primes for the largest qualifying p. */
        size_t ii = primes_upto;                  /* starting index: first > next_even‑1 */
        bool found = false;
        if (ii > 0) {
            for (ii = ii - 1; ; --ii) {
                uint64_t p   = prime_list[ii];
                uint64_t diff = next_even - p;
                if (is_prime_small((seq_t)diff)) {
                    size_t cnt = primes_upto - (ii + 1);   /* π(E‑1) – π(p) */
                    grow_a_arrays(cnt + 1);
                    if (!a_mask[cnt]) {
                        a_values[cnt] = (seq_t)next_even;
                        a_mask[cnt]   = true;
                    }
                    found = true;
                    break;   /* largest qualifying p found */
                }
                if (ii == 0) break;   /* exhausted primes */
            }
        }

        /* If the target count was produced, return it. */
        if (target < a_cap && a_mask[target]) return a_values[target];

        /* Advance to the next even candidate. */
        next_even += 2;
    }
}
