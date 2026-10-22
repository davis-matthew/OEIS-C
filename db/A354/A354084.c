/*
 * A354084  Primes p such that p mod 6 = p mod 8.
 * https://oeis.org/A354084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p with p mod 6 == p mod 8 (offset 1)
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   grow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq_t.h"

/* Growing utilities for arrays */
static void grow_uint64(uint64_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    *arr = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    if (!*arr) abort();
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) abort();       /* offset 1 – a(1) is defined */

    /* static state */
    static uint64_t *primes = NULL;      /* all primes so far          */
    static size_t primes_len = 0, primes_cap = 0;
    static seq_t *selected = NULL;       /* primes satisfying the rule */
    static size_t sel_len = 0, sel_cap = 0;
    static uint64_t next = 2;            /* next integer to test       */

    /* return cached result if available */
    if (n <= sel_len) return (seq_t)selected[n - 1];

    /* generate primes until we have n qualifying ones */
    while (sel_len < n) {
        uint64_t cand = next++;
        if (cand < 2) continue;

        bool isprime;
        if (cand == 2 || cand == 3) {
            isprime = true;
        } else if ((cand & 1) == 0) {
            isprime = false;
        } else {
            isprime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                uint64_t p = primes[i];
                if (p > cand / p) break;
                if (cand % p == 0) { isprime = false; break; }
            }
        }

        if (!isprime) continue;

        /* store prime */
        grow_uint64(&primes, &primes_cap, primes_len + 1);
        primes[primes_len++] = cand;

        /* check the congruence p mod 6 == p mod 8 */
        if (cand == 2 || cand == 3 ||
            (cand % 24 == 1) || (cand % 24 == 5)) {
            grow_seq_t(&selected, &sel_cap, sel_len + 1);
            selected[sel_len++] = (seq_t)cand;
        }
    }

    return (seq_t)selected[n - 1];
}
