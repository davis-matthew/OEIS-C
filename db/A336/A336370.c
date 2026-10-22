/*
 * A336370  Numbers k such that gcd(k, prime(k) + prime(k-1)) = 1.
 * https://oeis.org/A336370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that gcd(k, * prime(k) +
 *            prime(k-1)) == 1
 * Range:     u8 n <= 11, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  NOTE: The function relies on the gcd implementation from
 *  ../utils/gcd.h.  The gcd() function operates on values of type
 *  seq_t.  For indices that do not fit in seq_t the behaviour is
 *  undefined, but the test harness only requests terms that do
 *  fit in the configured seq_t type. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Helper: grow a dynamic array */
static void grow_array(size_t *cap, void **arr, size_t needed, size_t elem_size)
{
    if (*cap >= needed)
        return;
    size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
    while (newcap < needed)
        newcap <<= 1;
    void *tmp = realloc(*arr, newcap * elem_size);
    if (!tmp)
        abort();
    *arr = tmp;
    *cap = newcap;
}

/* ---------------------------------------------------------------------------
 * Prime generation (by trial division).  The primes are stored in a
 * static dynamic array of uint64_t.  We only need the (k)-th prime
 * during the sequence construction.
 * --------------------------------------------------------------------------- */

static uint64_t *prime_vals     = NULL;   /* prime_vals[i] == (i+1)-th prime */
static size_t   prime_cnt     = 0;
static size_t   prime_cap     = 0;

/* add the next prime to prime_vals */
static void add_next_prime(void)
{
    uint64_t cand;
    if (prime_cnt == 0)
        cand = 2;
    else if (prime_vals[prime_cnt - 1] == 2)
        cand = 3;
    else
        cand = prime_vals[prime_cnt - 1] + 2;  /* skip even numbers */

    for (;;) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_vals[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            grow_array(&prime_cap, (void **)&prime_vals, prime_cnt + 1, sizeof(uint64_t));
            prime_vals[prime_cnt++] = cand;
            return;
        }
        cand += 2;  /* test next odd number */
    }
}

/* Ensure that prime_vals contains at least (idx+1) primes (indexing from 0). */
static void ensure_primes_upto_idx(idx_t idx)
{
    while (prime_cnt <= (size_t)idx)
        add_next_prime();
}

/* ---------------------------------------------------------------------------
 * The sequence cache: seq_cache[i] holds the i-th value of the sequence
 * (1‑based indices).  last_k remembers the largest k that has already
 * been inspected.
 * --------------------------------------------------------------------------- */

static seq_t   *seq_cache   = NULL;   /* holds the sequence values */
static size_t   seq_cnt     = 0;      /* number of values cached   */
static size_t   seq_cap     = 0;      /* capacity of seq_cache     */
static idx_t    last_k      = 1;      /* last inspected k (1‑based) */

/* Expand the cache to have at least n terms */
static void ensure_seq_n_terms(idx_t n)
{
    if (seq_cnt >= (size_t)n)
        return;

    idx_t k = last_k + 1;             /* start one past the last inspected k */

    while (seq_cnt < (size_t)n) {
        /* we need prime(k) and prime(k-1) ==> indices k-1 and k-2 */
        ensure_primes_upto_idx(k - 1);
        ensure_primes_upto_idx(k - 2);

        seq_t pk   = (seq_t)prime_vals[(size_t)k - 1];
        seq_t pkm1 = (seq_t)prime_vals[(size_t)k - 2];

        /* gcd operates on seq_t values */
        uint64_t sum = (uint64_t)prime_vals[(size_t)k - 1] +
                       (uint64_t)prime_vals[(size_t)k - 2];
        seq_t sum_t = (seq_t)sum;

        if (gcd((seq_t)k, sum_t) == 1) {
            grow_array(&seq_cap, (void **)&seq_cache, seq_cnt + 1, sizeof(seq_t));
            seq_cache[seq_cnt++] = (seq_t)k;
        }
        ++k;
    }
    last_k = k - 1;
}

/* ---------------------------------------------------------------------------
 * Public function
 * --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but guard against zero */

    ensure_seq_n_terms(n);
    return seq_cache[(size_t)n - 1];
}
