/*
 * A338946  Lengths of Cunningham chains of the second kind that are sorted by
 *          first prime in the chain.
 * https://oeis.org/A338946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the Cunningham chain of the second kind lengths, sorted by the first prime in each chain.
   The function maintains static state so it works correctly for non‑monotonic calls.  It
   was chosen because explicit memoisation of all terms up to any n would consume too much
   memory, whereas searching for the next unused prime on demand is sufficiently fast
   (average chain length ≈ 4 for n≤10 000).  The algorithm is:
     • Keep a sorted list of all primes that have already appeared in some chain.
     • To obtain the next chain start, scan upwards from the last tested value until a
       prime that is not in the list is found.
     • Build the chain by repeatedly applying p ← 2 p − 1 until the result is composite.
     • Store the chain length in a cached array for future look‑ups.              */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static bool inited = false;

/* Cached chain lengths */
static seq_t *chain_arr = NULL;
static size_t chain_cap = 0;
static size_t chain_cnt = 0;

/* List of all primes that have appeared in any chain */
static idx_t *used_primes = NULL;
static size_t used_cap = 0;
static size_t used_cnt = 0;

/* Next candidate to test for primality */
static idx_t next_candidate = 2;

/* Ensure enough room in a dynamically sized array */
static void ensure_cap(size_t *cnt, size_t *cap, size_t item_size, void **array_ptr) {
    if (*cnt >= *cap) {
        size_t new_cap = (*cap == 0) ? 32 : (*cap << 1);
        void *tmp = realloc(*array_ptr, new_cap * item_size);
        if (!tmp) abort();
        *array_ptr = tmp;
        *cap = new_cap;
    }
}

/* Return true if x is in the used_primes list */
static bool used_contains(idx_t x) {
    for (size_t i = 0; i < used_cnt; ++i)
        if (used_primes[i] == x)
            return true;
    return false;
}

/* Add a prime to the used list */
static void add_used(idx_t p) {
    ensure_cap(&used_cnt, &used_cap, sizeof(idx_t), (void **)&used_primes);
    used_primes[used_cnt++] = p;
}

/* Ensure the state is set up once */
static void init(void) {
    if (inited) return;
    chain_cap = 0;
    chain_cnt = 0;
    chain_arr = NULL;
    used_cap = 0;
    used_cnt = 0;
    used_primes = NULL;
    inited = true;
}

/* Return the nth term of the sequence (1‑indexed) */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* Offset is 1; behaviour below is undefined */
    init();

    /* Compute more chain lengths until we have at least n entries */
    while (chain_cnt < n) {
        /* Find the next unused prime */
        idx_t start = next_candidate;
        for (; ; ++start) {
            if (!is_prime(start)) continue;
            if (used_contains(start)) continue;
            break;
        }
        add_used(start);
        next_candidate = start + 1;

        /* Build the Cunningham chain of the second kind */
        idx_t cur = start;
        size_t len = 1;
        for (;;) {
            idx_t next = cur * 2ULL - 1ULL;
            if (!is_prime(next)) break;
            add_used(next);
            ++len;
            cur = next;
        }

        /* Store the chain length */
        ensure_cap(&chain_cnt, &chain_cap, sizeof(seq_t), (void **)&chain_arr);
        chain_arr[chain_cnt++] = (seq_t)len;
    }

    return chain_arr[n - 1];
}
