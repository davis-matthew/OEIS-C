/*
 * A331634  a(n) is the greatest possible least part of any prime partition of
 *          n.
 * https://oeis.org/A331634
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 9076, u32 n <= 9076, u64 n <= 9076,
 *            u128 n <= 9076, big any n
 * Helpers:   build_primes_upto, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = maximum p such that n can be expressed as a sum of primes all ≥ p.
    This implementation uses a decreasing‑prime dynamic programme:
    prime partitions are built in descending order of the primes.
    For each n, the first (largest) prime that can reach n becomes
    the answer for that n.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"
#include "../utils/build_primes_upto.h"

/* static state shared between calls */
static seq_t *largest_min_prime = NULL;   /* answer table (index by n)   */
static size_t ans_cap            = 0;     /* capacity of largest_min_prime */
static idx_t   max_prepared      = 0;     /* largest n that has been prepared */

static uint8_t *reachable = NULL;
static size_t reachable_cap = 0;

/* Build the answer table up to and including n.
   The table is expanded only when n > max_prepared.  */
static void prepare_up_to(idx_t n)
{
    if (n <= max_prepared)
        return;   /* already prepared */

    /* Allocate or grow the answer array.  */
    size_t new_cap = (size_t)(n + 1);

    if (new_cap > ans_cap) {
        largest_min_prime = seq_realloc(largest_min_prime, ans_cap, new_cap);
        /* initialise the new entries to 0 */
        for (size_t i = ans_cap; i < new_cap; ++i)
            largest_min_prime[i] = (seq_t)0;
        ans_cap = new_cap;
    }

    /* Allocate or grow the reachable table.  */
    if (new_cap > reachable_cap) {
        reachable = (uint8_t *)realloc(reachable, new_cap * sizeof(uint8_t));
        for (size_t i = reachable_cap; i < new_cap; ++i)
            reachable[i] = 0;
        reachable_cap = new_cap;
    }

    /* Reset reachable up to n */
    for (size_t i = 0; i <= (size_t)n; ++i) reachable[i] = 0;
    reachable[0] = 1;   /* 0 is trivially reachable */

    /* Get the list of primes up to n.  */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(n, &primes, &primes_cnt);

    /* Process primes in decreasing order.  */
    for (size_t pos = primes_cnt; pos-- > 0; ) {
        idx_t p = (idx_t)primes[pos];

        /* Update reachable sums: add p to all previously reachable sums.  */
        for (idx_t s = p; s <= n; ++s) {
            if (reachable[(size_t)(s - p)])
                reachable[(size_t)s] = 1;
        }

        /* Assign answer for numbers newly reachable with this prime.  */
        for (idx_t s = p; s <= n; ++s) {
            if (reachable[(size_t)s] &&
                largest_min_prime[(size_t)s] == (seq_t)0)
                largest_min_prime[(size_t)s] = (seq_t)p;
        }
    }

    free(primes);
    max_prepared = n;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;  /* undefined for n < 2, offset is 2 */

    prepare_up_to(n);
    return largest_min_prime[(size_t)n];
}
