/*
 * A280010  Orders of consecutive clusters of non-Chen primes.
 * https://oeis.org/A280010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 2378, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280010 – Orders of consecutive clusters of non‑Chen primes.
   The function generates primes in order, identifies Chen primes
   (p is Chen when p+2 has at most two prime factors, counted with
   multiplicity), and records the lengths of consecutive run(s) of
   non‑Chen primes.  The clusters are closed by a Chen prime.  Results
   are cached so subsequent calls are O(1). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/count_prime_factors.h"

/* Static state for memoisation ********************************************/

static seq_t *cluster_len_arr = NULL;    /* holds the cluster lengths */
static size_t cluster_cap     = 0;        /* capacity of cluster_len_arr   */
static size_t cluster_cnt     = 0;        /* number of closed clusters     */
static size_t current_cluster = 0;        /* length of the cluster being built */
static idx_t  next_candidate = 2;        /* next integer to test for prime */

/* Ensure the dynamic array has room for at least 'needed' elements. */
static void
ensure_cluster_cap(size_t needed)
{
    if (needed <= cluster_cap)
        return;
    size_t new_cap = cluster_cap ? cluster_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cluster_len_arr, new_cap * sizeof(seq_t));
    cluster_len_arr = tmp;
    cluster_cap     = new_cap;
}

/* Return the next prime number, using is_prime() from the utils. */
static idx_t
next_prime(void)
{
    while (!is_prime(next_candidate))
        ++next_candidate;
    idx_t p = next_candidate;
    ++next_candidate;
    return p;
}

/* Main sequence function ***********************************************/

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 so callers can ignore it. */
    if (n < 1)
        return 0;

    /* Generate primes until the nth cluster has been closed and stored. */
    while (cluster_cnt < (size_t)n)
    {
        idx_t p = next_prime();            /* prime p */

        /* Determine if p is a Chen prime.  A Chen prime has p+2 with
           at most two prime factors (counted with multiplicity). */
        idx_t pp2 = p + 2;
        bool is_chen = (count_prime_factors((seq_t)pp2) <= 2);

        if (!is_chen)              /* non‑Chen prime: extend current cluster */
        {
            ++current_cluster;
        }
        else                       /* Chen prime: close the current cluster if any */
        {
            if (current_cluster > 0)
            {
                ++cluster_cnt;
                ensure_cluster_cap(cluster_cnt);
                cluster_len_arr[cluster_cnt - 1] = (seq_t)current_cluster;
                current_cluster = 0;
            }
            /* A Chen prime itself does not start a new cluster; it only
               separates clusters. */
        }
    }

    /* Return the requested cluster length (1‑based). */
    return cluster_len_arr[n - 1];
}
