/*
 * A301812  Numbers of the form p^2 - 1 where p is a prime of the form 3*k-1
 *          (A003627).
 * https://oeis.org/A301812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n^2 - 1, where p_n is the n‑th prime ≡ 2 (mod 3)
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Ensures that *arr has room for at least (*cnt + 1) elements. */
static void ensure_capacity(size_t *cnt, size_t *cap, uint64_t **arr)
{
    if (*cnt >= *cap) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 2;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Computes a(n).  n ≥ 1.  The implementation lazily generates all primes
   of the form 3k‑1 until the n‑th such prime is found.  */
seq_t a(idx_t n)
{
    /* static storage for generated primes */
    static uint64_t *primes_all     = NULL;
    static size_t primes_all_cnt    = 0;
    static size_t primes_all_cap    = 0;

    static uint64_t *primes_mod2    = NULL;
    static size_t primes_mod2_cnt   = 0;
    static size_t primes_mod2_cap   = 0;

    /* last_checked is the last integer that was tested for primality */
    static idx_t last_checked = 1; /* start before the first prime 2 */

    while (primes_mod2_cnt < (size_t)n) {
        /* find the next candidate prime */
        idx_t cand = last_checked + 1;
        if (cand < 2) cand = 2;          /* cover the case of n = 1 */
        if (cand > 2 && (cand & 1u) == 0) cand++; /* make it odd after 2 */

        /* test cand for primality using the primes we already have   */
        bool is_prime = true;
        for (size_t i = 0; i < primes_all_cnt; ++i) {
            uint64_t p = primes_all[i];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* store prime in the total list */
            ensure_capacity(&primes_all_cnt, &primes_all_cap, &primes_all);
            primes_all[primes_all_cnt++] = cand;

            /* if the prime is ≡ 2 (mod 3), keep it in the special list */
            if (cand % 3 == 2) {
                ensure_capacity(&primes_mod2_cnt, &primes_mod2_cap, &primes_mod2);
                primes_mod2[primes_mod2_cnt++] = cand;
            }
        }

        /* remember where we left off */
        last_checked = cand;
    }

    /* p_n is the n‑th element in primes_mod2 (0‑based index).  */
    seq_t p = (seq_t)primes_mod2[(size_t)n - 1];
    return p * p - (seq_t)1;
}
