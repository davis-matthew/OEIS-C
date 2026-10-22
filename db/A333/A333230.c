/*
 * A333230  Positions of weak ascents in the sequence of differences between
 *          primes.
 * https://oeis.org/A333230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that prime(k+2) - 2*prime(k+1) +
 *            prime(k) >= 0 (OEIS A333230). The function computes primes on
 *            demand, stores indices that satisfy the condition, and returns
 *            the requested term
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *primes = NULL;      /* array of generated primes, 0‑based (prime[0] = 2) */
static size_t   prime_cnt = 0;      /* number of primes stored */
static size_t   prime_cap = 0;      /* allocated capacity for primes */

static void grow_primes(size_t need)
{
    if (prime_cnt >= need)
        return;

    /* enlarge capacity if necessary */
    if (prime_cnt >= prime_cap) {
        size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
        while (new_cap < need)
            new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        primes = tmp;
        prime_cap = new_cap;
    }

    /* initialise with first prime if needed */
    if (prime_cnt == 0) {
        primes[prime_cnt++] = 2;   /* prime[0] = 2   */
        /* next odd candidate to test starts at 3 */
    }

    static uint64_t next_odd = 3;   /* static to preserve across calls */
    while (prime_cnt < need) {
        uint64_t cand = next_odd;
        bool isprime = true;
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p = primes[j];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime)
            primes[prime_cnt++] = cand;
        next_odd += 2;
    }
}

static idx_t *weak_k = NULL;      /* indices k that satisfy the inequality */
static size_t weak_cnt = 0;       /* number of such indices found */
static size_t weak_cap = 0;       /* allocated capacity for weak_k */
static idx_t   current_k = 0;     /* last k tested (starts at 0) */

static void grow_weak_arr(size_t need)
{
    while (weak_cnt < need) {
        ++current_k;
        /* need primes up to index current_k+1 (zero‑based) -> count = current_k+2 */
        grow_primes((size_t)current_k + 2);

        int64_t delta2 =
            (int64_t)primes[(size_t)current_k + 1]          /* prime(k+2) */
            - 2 * (int64_t)primes[(size_t)current_k]         /* 2*prime(k+1) */
            + (int64_t)primes[(size_t)current_k - 1];        /* prime(k)   */

        if (delta2 >= 0) {
            if (weak_cnt >= weak_cap) {
                size_t new_cap = (weak_cap == 0) ? 16 : weak_cap * 2;
                idx_t *tmp = (idx_t *)realloc(weak_k, new_cap * sizeof(idx_t));
                weak_k = tmp;
                weak_cap = new_cap;
            }
            weak_k[weak_cnt++] = current_k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    grow_weak_arr((size_t)n);
    return (seq_t)weak_k[(size_t)n - 1];
}
