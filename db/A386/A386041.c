/*
 * A386041  Primes having only {0, 2, 3, 4} as digits.
 * https://oeis.org/A386041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 71, u32 n <= 778, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_t_max_integers
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the primes whose decimal representation uses only the
 * digits {0,2,3,4}.  The function returns the n‑th such prime (a(1) = 2).  It
 * enumerates all numbers formed with those digits in increasing order by a
 * breadth‑first traversal, checks primality with the supplied
 * is_prime_big routine, and caches all found primes.
 * The implementation works for integral seq_t types up to 64 bits, and also
 * for larger arbitrary‑precision integer classes.  For types that cannot
 * hold the n‑th prime, the function returns 0 after exhausting all
 * representable candidates. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

#include "../utils/is_prime_big.h"
#include "../utils/seq_t_max_integers.h"

/* -------------------------------------------------------------------------- */
/*  Helpers and global tables                                                */
/* -------------------------------------------------------------------------- */

static seq_t   *prime_cache     = NULL;   /* array of found primes      */
static size_t  prime_len       = 0;        /* number of stored primes    */
static size_t  prime_cap       = 0;        /* capacity of prime_cache    */

static seq_t   *queue          = NULL;   /* BFS queue of candidates   */
static size_t  q_start         = 0;        /* head index                */
static size_t  q_end           = 0;        /* tail index                */
static size_t  q_cap           = 0;        /* capacity of queue         */

static bool    initialized = false;        /* one‑time init flag        */

static const unsigned char allowed_digits[4] = {0, 2, 3, 4};

/* ensure queue has enough capacity; double if full */
static void ensure_queue_capacity(void)
{
    if (q_end == q_cap) {
        size_t new_cap = q_cap ? q_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(queue, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        queue = tmp;
        q_cap = new_cap;
    }
}

/* ensure prime cache has enough capacity; double if full */
static void ensure_prime_capacity(void)
{
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        prime_cache = tmp;
        prime_cap   = new_cap;
    }
}

/* shift queue contents to reclaim space when the head has advanced far */
static void shift_queue_if_needed(void)
{
    if (q_start == 0) return;
    if (q_start < q_cap / 4) return;
    size_t remaining = q_end - q_start;
    memmove(queue, queue + q_start, remaining * sizeof(seq_t));
    q_start = 0;
    q_end   = remaining;
}

/* -------------------------------------------------------------------------- */
/*  Main function                                                             */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (!initialized) {
        /* initialise queue with the single‑digit admissible primes */
        queue = (seq_t *)malloc(64 * sizeof(seq_t));
        if (!queue) abort();
        q_cap = 64;
        /* enqueue 2,3,4 */
        queue[0] = (seq_t)2;
        queue[1] = (seq_t)3;
        queue[2] = (seq_t)4;
        q_start = 0;
        q_end   = 3;

        prime_cache = NULL;
        prime_len   = 0;
        prime_cap   = 0;
        initialized = true;
    }

    const uintmax_t max_int = seq_t_max_integers();

    while (prime_len < (size_t)n && q_start < q_end) {
        seq_t cur = queue[q_start++];
        /* check practical overflow for integral types */
        if (sizeof(seq_t) <= 8) {
            uintmax_t val = (uintmax_t)cur;
            if (val > max_int) continue;   /* skip unrepresentable value */
        }

        if (is_prime_big(cur)) {
            ensure_prime_capacity();
            prime_cache[prime_len++] = cur;
        }

        /* generate children by appending each allowed digit */
        for (int i = 0; i < 4; ++i) {
            unsigned char d = allowed_digits[i];
            /* For the initial digits we already ensured no leading zero.   */
            seq_t child;
            if (sizeof(seq_t) <= 8) {
                unsigned __int128 temp = (unsigned __int128)cur * 10 + d;
                if ((uintmax_t)temp > max_int) continue;
                child = (seq_t)temp;
            } else {
                child = cur * 10 + d;
            }
            ensure_queue_capacity();
            queue[q_end++] = child;
        }

        shift_queue_if_needed();
    }

    /* If we ran out of representable candidates before reaching n, return 0. */
    if (prime_len < (size_t)n) return (seq_t)0;
    return prime_cache[(size_t)n - 1];
}
