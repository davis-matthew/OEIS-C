/*
 * A329567  For all n >= 0, exactly seven sums are prime among a(n+i) +
 *          a(n+j), 0 <= i < j < 6: lexicographically earliest such sequence
 *          of distinct nonnegative numbers.
 * https://oeis.org/A329567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence defining the lexicographically earliest * permutation
 *            of the non‑negative integers with 7 primes among * the 15
 *            pairwise sums of any 6 consecutive terms. * a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=4, a(5)=8. * For n≥6: * let
 *            B={a(n-6),...,a(n-1)}; * let t = number of primes among
 *            {a(n-6)+a...
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   isPrimeGeneric, used_in_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/used_in_seq.h"

static seq_t *seq      = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

/* Ensure the dynamic array has room for at least `needed` elements. */
static void ensure_capacity(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_ptr)
        exit(1);   /* Allocation failed */
    seq = new_ptr;
    seq_cap = new_cap;
}

/* Count how many of the 5 sums (val + each of the 5 last terms) are prime. */
static int count_prime_sums(const seq_t *last5, seq_t val)
{
    int cnt = 0;
    for (size_t i = 0; i < 5; ++i)
        if (isPrimeGeneric(last5[i] + val))
            ++cnt;
    return cnt;
}

/* Public sequence function, memoised. */
seq_t a(idx_t n)
{
    while (seq_len <= (size_t)n) {
        ensure_capacity(seq_len + 1);

        /* Initialise first six values explicitly. */
        if (seq_len < 6) {
            static const seq_t base[6] = {0, 1, 2, 3, 4, 8};
            seq[seq_len] = base[seq_len];
            ++seq_len;
            continue;
        }

        /* Compute a(n) for n >= 6. */
        const seq_t *last5   = &seq[seq_len - 5];    /* a(n‑5)…a(n‑1) */
        seq_t       base_elem = seq[seq_len - 6];    /* a(n‑6)      */
        int         target    = count_prime_sums(last5, base_elem);

        seq_t cand = 0;
        while (true) {
            if (!used_in_seq(seq, seq_len, cand)) {
                if (count_prime_sums(last5, cand) == target)
                    break;
            }
            ++cand;
        }

        seq[seq_len++] = cand;
    }
    return seq[(size_t)n];
}
