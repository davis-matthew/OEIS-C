/*
 * A344891  Divide the primes into subsets of lengths given by successive
 *          primes, then reverse the order of terms in each subset.
 * https://oeis.org/A344891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Constructive definition: a(n) is the n-th prime obtained by
   grouping the primes into blocks with lengths equal to the
   successive primes, and writing each block in decreasing order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- prime storage --------------------------------------- */
static uint64_t *primes = NULL;     /* primes[0] = 2, primes[1] = 3, ...        */
static size_t   primes_cap = 0;     /* capacity of primes array                */
static size_t   primes_cnt = 0;     /* number of primes currently stored       */

/* Ensure that at least 'need' primes are stored.               */
static void
ensure_primes_at_least(size_t need)
{
    if (primes_cnt >= need)
        return;

    if (primes_cap < need) {
        size_t new_cap = primes_cap ? primes_cap << 1 : 16;
        while (new_cap < need)
            new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp)
            abort();
        primes = tmp;
        primes_cap = new_cap;
    }

    uint64_t candidate = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
    while (primes_cnt < need) {
        bool isprime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            primes[primes_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* ---------- cumulative block sizes ---------------------------- */
static idx_t *cum = NULL;          /* cum[i] = total terms up to block i (1‑based) */
static size_t cum_cap = 0;         /* capacity of cum array                     */
static size_t cum_cnt = 0;         /* number of cumulative entries              */

/* Ensure that the cumulative array reaches at least 'target' terms. */
static void
ensure_cumulative_up_to(idx_t target)
{
    if (cum_cnt > 0 && cum[cum_cnt - 1] >= target)
        return;

    for (;;) {
        /* Ensure the prime that determines the current block size exists. */
        ensure_primes_at_least(cum_cnt + 1);

        idx_t prev = cum_cnt == 0 ? 0 : cum[cum_cnt - 1];
        uint64_t block_size = primes[cum_cnt];          /* length of this block */
        idx_t new_cum = prev + (idx_t)block_size;

        if (cum_cap <= cum_cnt) {
            size_t new_cap = cum_cap ? cum_cap << 1 : 16;
            while (new_cap <= cum_cnt)
                new_cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
            if (!tmp)
                abort();
            cum = tmp;
            cum_cap = new_cap;
        }

        cum[cum_cnt++] = new_cum;
        if (new_cum >= target)
            break;
    }
}

/* ---------- sequence function ---------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset begins at 1 */

    /* Build enough blocks to cover n. */
    ensure_cumulative_up_to(n);

    /* Find the block that contains position n.                   */
    size_t k = 0;
    while (k < cum_cnt && n > cum[k])
        ++k;                           /* block index (0‑based) */

    idx_t prev_cum   = (k == 0) ? 0 : cum[k - 1];
    idx_t block_len  = cum[k] - prev_cum;              /* length of block k */
    idx_t pos_in_blk = n - prev_cum;                   /* 1‑based position in block */

    /* The block is written in decreasing order, so the prime index
       (1‑based) is: previous primes count + block_len - pos_in_blk + 1. */
    idx_t prime_index = prev_cum + block_len - pos_in_blk + 1;

    /* Ensure we have enough primes. */
    ensure_primes_at_least(prime_index);

    uint64_t prime_value = primes[prime_index - 1];
    return (seq_t)prime_value;
}
