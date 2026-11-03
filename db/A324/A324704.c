/*
 * A324704  Lexicographically earliest sequence containing 1 and all numbers >
 *          2 divisible by prime(m) for some m already in the sequence.
 * https://oeis.org/A324704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes the lexicographically earliest sequence containing 1
 *            and all numbers > 2 divisible by prime(m) for some m already in
 *            the sequence, by incremental construction
 * Range:     u8 n <= 59, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static void grow_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    *arr = mem_resize(seq_t, *arr, new_cap);
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static seq_t *allowed = NULL;
    static size_t allowed_len = 0, allowed_cap = 0;

    if (!initialized)
    {
        /* Initialize sequence array with a(1) = 1 */
        seq_cap = 8;
        seq = mem_new(seq_t, seq_cap);
        seq[0] = (seq_t)1;
        seq_len = 1;

        /* Start with allowed primes: prime(1) = 2 */
        allowed_cap = 8;
        allowed = mem_new(seq_t, allowed_cap);
        allowed[0] = get_prime((idx_t)1);  /* 2 */
        allowed_len = 1;

        initialized = true;
    }

    while (seq_len < n)
    {
        seq_t last = seq[seq_len - 1];
        seq_t candidate = last + (seq_t)1;
        if (candidate <= (seq_t)2) candidate = (seq_t)3;

        /* Find next number divisible by any allowed prime */
        for (;;)
        {
            bool ok = false;
            for (size_t i = 0; i < allowed_len; ++i)
            {
                if (candidate % allowed[i] == (seq_t)0)
                {
                    ok = true;
                    break;
                }
            }
            if (ok) break;
            ++candidate;
        }

        /* Append the new term */
        grow_array(&seq, &seq_cap, seq_len + 1);
        seq[seq_len++] = candidate;

        /* Add prime(candidate) to allowed set */
        idx_t cand_idx = (idx_t)candidate;
        seq_t new_prime = get_prime(cand_idx);
        grow_array(&allowed, &allowed_cap, allowed_len + 1);
        allowed[allowed_len++] = new_prime;
    }

    return seq[n - 1];   /* a is 1‑indexed */
}
