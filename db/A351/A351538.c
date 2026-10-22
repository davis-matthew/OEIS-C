/*
 * A351538  Numbers k such that both k and sigma(k) are congruent to 2 modulo
 *          4 and the 3-adic valuation of sigma(k) is exactly 1.
 * https://oeis.org/A351538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   computed by searching for k ≡ 2 (mod 4) with sigma(k) ≡ 2 (mod
 *            4) and v₃(sigma(k)) = 1 (i.e. 3‖sigma(k))
 * Range:     u8 n <= 4, u16 n <= 527, u32 n <= 9958, u64 n <= 9958,
 *            u128 n <= 9958, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sigma_of.h"

/* cached sequence of found terms */
static seq_t *seq_cache = NULL;      /* array of seq_t */
static size_t cache_size = 0;        /* allocated capacity */
static size_t cache_cnt  = 0;        /* number of terms stored */
static idx_t next_k = 2;             /* next k to test */

/* append a term to the cache, growing it if necessary */
static void append_cache(seq_t val)
{
    if (cache_cnt >= cache_size) {
        size_t new_size = (cache_size == 0) ? 16 : cache_size * 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                 /* out‑of‑memory: terminate */
        seq_cache = tmp;
        cache_size = new_size;
    }
    seq_cache[cache_cnt++] = val;
}

seq_t a(idx_t n)
{
    /* Build the cache until we have at least n terms. */
    while (cache_cnt < n) {
        /* skip k not ≡ 2 (mod 4) */
        if (next_k % 4 != 2) {
            ++next_k;
            continue;
        }

        seq_t sigma = sigma_of(next_k);

        /* sigma must satisfy the congruence and 3‑adic constraints */
        if (sigma % 4 != 2) {
            ++next_k;
            continue;
        }
        if (sigma % 3 != 0 || sigma % 9 == 0) {
            ++next_k;
            continue;
        }

        /* term found – store it */
        append_cache((seq_t)next_k);
        ++next_k;          /* continue from the next integer */
    }

    return seq_cache[n - 1];
}
