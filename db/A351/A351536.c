/*
 * A351536  Odd numbers for which sigma(k) is congruent to 2 modulo 4 and the
 *          3-adic valuations of k and sigma(k) are equal.
 * https://oeis.org/A351536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A351536: odd numbers k such that σ(k) ≡ 2 (mod 4) and v₃(k) = v₃(σ(k))
 *  The function below builds the sequence by brute‑force testing odd integers
 *  in ascending order.  Results are cached, so repeated calls in any order
 *  are efficient.  All arithmetic is performed in the provided integer
 *  types `seq_t` and `idx_t` (e.g. uint64_t) to satisfy the build system.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/sigma_of.h"   /* sigma_of(idx_t) -> seq_t */

/* Exponent of 3 in the prime factorisation of x */
static seq_t v3(seq_t x)
{
    seq_t v = 0;
    while ((x % (seq_t)3) == (seq_t)0) {
        v++;
        x /= (seq_t)3;
    }
    return v;
}

/*  a(n) returns the n‑th term of the sequence (n ≥ 1).  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* array of found terms */
    static size_t cap   = 0;     /* allocated capacity */
    static size_t len   = 0;     /* number of terms stored */
    static idx_t last_candidate = 1;  /* odd integer from which we resume searching */

    /* grow cache until it contains at least n terms */
    while (len < n) {
        /* increase the allocation if necessary */
        if (cap == 0) {
            cap = 128;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
        } else if ((len + 1) > cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp) { cache = tmp; cap = new_cap; }
        }

        /* search for the next valid odd integer */
        for (idx_t k = last_candidate + 2; ; k += 2) {
            seq_t sigma = sigma_of(k);
            if (((sigma & (seq_t)3) == (seq_t)2) &&
                (v3((seq_t)k) == v3(sigma))) {
                cache[len++] = (seq_t)k;
                last_candidate = k;
                break;  /* found the next term */
            }
        }
    }

    return cache[n - 1];
}
