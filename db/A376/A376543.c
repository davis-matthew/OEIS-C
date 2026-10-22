/*
 * A376543  Numbers of the form p^e * q^f with p, q distinct primes = 3 mod 4
 *          and e and f both odd.
 * https://oeis.org/A376543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   numbers of the form p^e * q^f with p, q distinct primes ≡ 3 (mod
 *            4) and e,f odd
 * Range:     u8 n <= 19, u16 n <= 3936, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_blum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_blum.h"

/* Ensure that an array has capacity at least min_cap. */
static void ensure_capacity(size_t min_cap, seq_t **arr, size_t *cap)
{
    if (*cap < min_cap) {
        size_t new_cap = (*cap == 0 ? 16 : (*cap << 1));
        if (new_cap < min_cap)
            new_cap = min_cap;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t next_candidate = 3;   /* start checking from 3 */

    while (seq_len < n)
    {
        uint64_t m = (uint64_t)next_candidate;
        if (is_blum(m))
        {
            ensure_capacity(seq_len + 1, &seq, &seq_cap);
            seq[seq_len++] = (seq_t)m;
        }
        ++next_candidate;
    }

    return seq[n - 1];
}
