/*
 * A249053  Defined by (i) a(1)=1; (ii) if you move a(n) steps to the right
 *          you must reach a prime; (iii) a(n) = smallest unused composite
 *          number greater than a(n-1), unless a(n) is required to be prime by
 *          (ii), in which case a(n) is the smallest unused prime greater than
 *          a(n-1).
 * https://oeis.org/A249053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249053
 * a(1) = 1.
 * Let prime_req[i] be true if a(i) must be prime (because some earlier
 * a(k) forces a(k+a(k)) to be prime).
 * For n>1:
 *   if prime_req[n] is true: a(n) = smallest unused prime > a(n-1)
 *   else: a(n) = smallest unused composite > a(n-1)
 *   In all cases a(n) marks that a(n+ a(n)) must be prime.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static tables shared between calls.  All indices are 1‑based. */
    static seq_t *seq      = NULL;   /* sequence values */
    static size_t seq_cap  = 0;      /* allocated capacity for seq   */
    static idx_t  seq_len  = 0;      /* highest index computed       */

    static uint8_t *used    = NULL;  /* used[v] = 1 if value v already used */
    static size_t  used_cap = 0;     /* capacity of used               */

    static uint8_t *prime_req = NULL;/* prime_req[i] = 1 if a(i) must be prime */
    static size_t  prime_req_cap = 0;/* capacity */

    /* ------------------------------------------------------------------ */
    /* Ensure tables have the required capacity.                          */
    /* ------------------------------------------------------------------ */

    /* sequence array */
    if (!seq_cap) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }
    if (n >= seq_cap) {
        size_t new_cap = seq_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* prime requirement array */
    if (!prime_req_cap) {
        prime_req_cap = 16;
        prime_req = (uint8_t *)calloc(prime_req_cap, sizeof(uint8_t));
    }
    if (n >= prime_req_cap) {
        size_t new_cap = prime_req_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        prime_req = (uint8_t *)realloc(prime_req, new_cap * sizeof(uint8_t));
        memset(prime_req + prime_req_cap, 0,
               (new_cap - prime_req_cap) * sizeof(uint8_t));
        prime_req_cap = new_cap;
    }

    /* used‑value array */
    if (!used_cap) {
        used_cap = 16;
        used = (uint8_t *)calloc(used_cap, sizeof(uint8_t));
        used[1] = 1;          /* a(1) = 1 is used */
    }

    /* ------------------------------------------------------------------ */
    /* Initialise base case if necessary.                                 */
    /* ------------------------------------------------------------------ */
    if (seq_len == 0) {
        seq_len = 1;
        seq[1] = (seq_t)1;
        /* a(1) forces a(1+1)=a(2) to be prime. */
        if (prime_req_cap < 3) { /* ensure index 2 exists */
            size_t new_cap = prime_req_cap ? prime_req_cap << 1 : 16;
            while (new_cap <= 2) new_cap <<= 1;
            prime_req = (uint8_t *)realloc(prime_req, new_cap * sizeof(uint8_t));
            memset(prime_req + prime_req_cap, 0,
                   (new_cap - prime_req_cap) * sizeof(uint8_t));
            prime_req_cap = new_cap;
        }
        prime_req[2] = 1;
    }

    /* ------------------------------------------------------------------ */
    /* Build the sequence up to index n if needed.                        */
    /* ------------------------------------------------------------------ */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t prev = seq[i-1];
        bool needPrime = prime_req[i] != 0;

        /* Find the smallest unused candidate > prev that satisfies the rule. */
        seq_t candidate = prev + (seq_t)1;
        for (;;) {
            /* Grow used array if necessary. */
            if ((size_t)candidate >= used_cap) {
                size_t new_cap = used_cap ? used_cap << 1 : 16;
                while (new_cap <= (size_t)candidate) new_cap <<= 1;
                used = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
                memset(used + used_cap, 0,
                       (new_cap - used_cap) * sizeof(uint8_t));
                used_cap = new_cap;
            }

            /* Skip already used numbers. */
            if (used[(size_t)candidate]) { ++candidate; continue; }

            /* Apply prime/composite restriction. */
            if (needPrime) {
                if (!is_prime_big(candidate)) { ++candidate; continue; }
            } else {
                if (candidate <= (seq_t)1 || is_prime_big(candidate)) {
                    ++candidate; continue;
                }
            }
            /* Candidate is suitable. */
            break;
        }

        /* Record the chosen value. */
        seq[i] = candidate;
        used[(size_t)candidate] = 1;

        /* a(i + candidate) must be prime. */
        idx_t target = i + (idx_t)candidate;
        if (target >= prime_req_cap) {
            size_t new_cap = prime_req_cap;
            while (new_cap <= target) new_cap <<= 1;
            prime_req = (uint8_t *)realloc(prime_req,
                                           new_cap * sizeof(uint8_t));
            memset(prime_req + prime_req_cap, 0,
                   (new_cap - prime_req_cap) * sizeof(uint8_t));
            prime_req_cap = new_cap;
        }
        prime_req[target] = 1;
    }

    seq_len = n;
    return seq[n];
}
