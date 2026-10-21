/*
 * A280084  1 together with the Pythagorean primes.
 * https://oeis.org/A280084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 1 if n = 1, otherwise the (n‑1)‑th prime congruent to 1
 *            mod 4
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a cache of primes of the form 4k+1.
   It grows the cache on demand and uses the utilities
   seq_realloc (for arbitrary precision) and is_prime_big
   (principal primality test). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"       /* seq_realloc */
#include "../utils/is_prime_big.h"      /* is_prime_big */

static seq_t *p_list     = NULL;   /* array holding the primes of the form 4k+1 */
static size_t p_count    = 0;      /* number of primes stored so far */
static size_t p_capacity = 0;      /* current capacity of p_list */
static uint64_t next_candidate = 5;/* next candidate ≡ 1 (mod 4) */

/* ------------------------------------------------------------------
 * Return a(idx_t n): nth term of sequence A280084.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;                 /* a(1) = 1 */

    /* Number of 4k+1 primes that must be found. */
    const idx_t need = n - 1;            /* 1‑based count of required primes */

    /* Grow the cache until it contains at least 'need' primes. */
    while (p_count < (size_t)need) {
        /* Grow storage if necessary. */
        if (p_count == p_capacity) {
            const size_t new_cap = p_capacity ? p_capacity * 2 : 16;
            seq_t *tmp = seq_realloc(p_list, p_capacity, new_cap);
            if (!tmp)
                abort();                  /* allocation failed */
            p_list     = tmp;
            p_capacity = new_cap;
        }

        /* Find the next 4k+1 prime. */
        for (;;) {
            const uint64_t cand = next_candidate;
            next_candidate += 4;          /* next odd number ≡ 1 (mod 4) */

            const seq_t cand_seq = (seq_t)cand;
            if (is_prime_big(cand_seq)) {
                p_list[p_count++] = cand_seq;
                break;
            }
        }
    }

    /* Return the (need‑1)‑th element (zero‑based index). */
    return p_list[(size_t)need - 1];
}
