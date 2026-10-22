/*
 * A304455  Number of steps in the reduction to a multiset of size 1 of the
 *          multiset of prime factors of n, obtained by repeatedly taking the
 *          multiset of multiplicities.
 * https://oeis.org/A304455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9650, u16 n <= 9650, u32 n <= 9650, u64 n <= 9650,
 *            u128 n <= 9650, big any n
 * Helpers:   sort_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A304455: Number of steps in the reduction to a multiset of size 1
   of the multiset of prime factors of n, obtained by repeatedly taking the
   multiset of multiplicities.  The function implements the definition
   directly; it performs a simple trial division factorisation and then
   reduces the multiset until its size becomes 1.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/sort_seq.h"

seq_t a(idx_t n)
{
    /* Special cases: a(1) = 0 and primes have no reduction steps. */
    if (n <= 3ULL) return (seq_t)0;

    /* Factorise n: gather distinct primes and their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t num = 0;
    uint64_t tmp = (uint64_t)n;

    for (uint64_t p = 2ULL; p <= tmp / p; ++p) {
        if (tmp % p == 0ULL) {
            uint64_t cnt = 0;
            while (tmp % p == 0ULL) {
                tmp /= p;
                ++cnt;
            }
            primes[num] = p;
            exps[num] = cnt;
            ++num;
        }
    }
    if (tmp > 1ULL) {
        primes[num] = tmp;
        exps[num] = 1;
        ++num;
    }

    /* Total number of prime factors (with multiplicity). */
    uint64_t total = 0;
    for (size_t i = 0; i < num; ++i) total += exps[i];

    /* If there is at most one prime factor, no reduction is needed. */
    if (total <= 1ULL) return (seq_t)0;

    /* Current multiset: the exponents of each distinct prime. */
    seq_t cur[128];
    size_t cur_size = num;
    for (size_t i = 0; i < num; ++i) cur[i] = (seq_t)exps[i];

    seq_t steps = 1;         /* first reduction: primes → exponents */

    seq_t nxt[128];
    size_t nxt_size;

    while (cur_size > 1) {
        /* Sort to group identical values together. */
        sort_seq(cur, cur_size);

        /* Build the next multiset: multiplicities of current values. */
        nxt_size = 0;
        for (size_t i = 0; i < cur_size; ) {
            seq_t val = cur[i];
            size_t j = i + 1;
            while (j < cur_size && cur[j] == val) {
                ++j;
            }
            nxt[nxt_size++] = (seq_t)(j - i);   /* count of occurrences */
            i = j;
        }

        /* Prepare for next iteration. */
        cur_size = nxt_size;
        for (size_t i = 0; i < cur_size; ++i) cur[i] = nxt[i];
        ++steps;
    }

    return steps;
}
