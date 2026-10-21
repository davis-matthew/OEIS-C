/*
 * A264962  Pulsating Checkpoint Sequence: use powers of 2 as checkpoints;
 *          place powers of 2, starting with 4, with spacing equal to the
 *          previous power of 2. Whenever we encounter a checkpoint, we jump
 *          over it; otherwise, we insert four numbers into the sequence: 2p,
 *          p, 3p, and 3p+3, where p is the smallest...
 * https://oeis.org/A264962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pulsating Checkpoint Sequence (OEIS A264962)
   Computes a(n) by incremental generation.  
   Checkpoints are powers of two: 2^k (k ≥ 2) are placed at
   positions 2^k + k − 5.  Between checkpoints we output blocks
   of four numbers [2p, p, 3p, 3p+3] where p is the smallest odd prime
   not yet encountered.  The routine is memoised so any call order
   works and each call works in O(n). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- small prime test ---------- */
static bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

/* ---------- static state for the sequence ---------- */
static seq_t *seq_cache   = NULL;
static idx_t  cache_size  = 0;   /* number of cached terms */
static idx_t  cache_cap   = 0;   /* allocated capacity */

/* checkpoint parameters */
static idx_t     k = 2;                    /* current power exponent */
static idx_t     next_k = 3;               /* next exponent */
static seq_t     next_checkpoint_value;    /* value of next checkpoint */
static idx_t     next_checkpoint_pos;     /* position of next checkpoint */

/* block / prime parameters */
static int      block_counter = 0;         /* 0..3 offset within a block */
static uint64_t last_prime    = 1;         /* last emitted odd prime */
static uint64_t current_prime = 0;         /* prime for current block */

/* ---------- helper: grow cache as needed ---------- */
static void ensure_cache(idx_t n_needed)
{
    if (cache_size >= n_needed) return;
    if (cache_cap == 0) {
        cache_cap = 64;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap < n_needed) {
        size_t new_cap = cache_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_arr) {
            /* fatal: memory allocation failed */
            exit(1);
        }
        seq_cache = new_arr;
        cache_cap = new_cap;
    }
}

/* ---------- initialise static variables once ---------- */
static void init_sequence(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    cache_size = 1;
    cache_cap  = 64;
    seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    seq_cache[0] = (seq_t)4;                 /* a(1) = 4 */

    k = 2;
    next_k = 3;
    next_checkpoint_value = (seq_t)(1ULL << next_k);
    next_checkpoint_pos = (idx_t)((1ULL << next_k) + next_k - 5);

    block_counter   = 0;
    last_prime      = 1;                     /* nothing before 3 */
    current_prime   = 0;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1 */

    init_sequence();

    /* Fast return if already cached */
    if (n <= cache_size) {
        return seq_cache[n - 1];
    }

    /* Grow cache to at least n */
    ensure_cache(n);

    /* Generate sequence values until we reach position n */
    while (cache_size < n) {

        /* Next position to fill */
        idx_t next_pos = cache_size + 1;

        if (next_pos == next_checkpoint_pos) {
            /* Insert the checkpoint */
            seq_cache[cache_size++] = next_checkpoint_value;

            /* Advance to next checkpoint */
            k = next_k;
            next_k++;
            next_checkpoint_value = (seq_t)(1ULL << next_k);
            next_checkpoint_pos   = (idx_t)((1ULL << next_k) + next_k - 5);

            block_counter = 0;     /* new block starts after a checkpoint */
        } else {
            /* Produce block entry */
            if (block_counter == 0) {
                /* fetch next odd prime */
                uint64_t cand = last_prime + 2;
                while (!is_prime_small(cand)) cand += 2;
                current_prime = cand;
                last_prime = cand;
            }

            seq_t value;
            switch (block_counter) {
                case 0: value = (seq_t)(2ULL * current_prime);     break;
                case 1: value = (seq_t)(current_prime);           break;
                case 2: value = (seq_t)(3ULL * current_prime);     break;
                default: value = (seq_t)(3ULL * current_prime + 3ULL); break;
            }

            seq_cache[cache_size++] = value;

            block_counter++;
            if (block_counter == 4) block_counter = 0;
        }

        /* Ensure capacity for next insertion */
        if (cache_size > cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!new_arr) exit(1);
            seq_cache = new_arr;
            cache_cap = new_cap;
        }
    }

    return seq_cache[n - 1];
}
