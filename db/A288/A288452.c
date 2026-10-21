/*
 * A288452  Pseudoperfect totient numbers: numbers n such that equal the sum
 *          of a subset of their iterated phi(n).
 * https://oeis.org/A288452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the pseudoperfect totient numbers A288452.
 * For a given n the function returns the nth term (with offset 1),
 * i.e. a(1)=3, a(2)=5, …
 *
 * Definition: n is a pseudoperfect totient number if
 *   n == sum of some subset of the iterated totient chain
 *   φ(n), φ(φ(n)), …, 1
 *
 * The algorithm tests each odd integer in order, using a bitset subset‑sum
 * dynamic programming to decide whether the value can be achieved as a
 * subset sum of its chain.  The list of found terms is memoised so that
 * multiple calls to a(idx_t) are fast and independent of call order.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/calc_phi.h"
#include "../utils/ensure_capacity.h"

/* ----------  Subset‑sum bitset helper  ---------- */

/* Static buffers for the bitset DP. */
static uint64_t *bitset = NULL;          /* reachable sums */
static uint64_t *bitset_tmp = NULL;      /* temporary buffer for shifting */
static size_t bitset_cap = 0;            /* number of 64‑bit words allocated */

/* Ensure the bitset can hold all sums up to the specified target. */
static void ensure_bitset_cap(size_t words)
{
    if (words <= bitset_cap)
    {
        /* Reset to zero. */
        memset(bitset, 0, bitset_cap * sizeof(uint64_t));
        return;
    }
    free(bitset);
    free(bitset_tmp);
    bitset = (uint64_t *)calloc(words, sizeof(uint64_t));
    bitset_tmp = (uint64_t *)calloc(words, sizeof(uint64_t));
    bitset_cap = words;
}

/* Test whether target can be represented as a subset sum of 'chain'. */
static bool subset_sum_possible(seq_t target, const seq_t *chain, size_t len)
{
    if (target == 0) return true;
    size_t words = (size_t)((target >> 6) + 1);   /* target/64 + 1 */
    ensure_bitset_cap(words);

    /* Only sum 0 is reachable initially. */
    bitset[0] = 1ULL;
    for (size_t i = 1; i < words; ++i) bitset[i] = 0ULL;

    for (size_t ci = 0; ci < len; ++ci)
    {
        seq_t val = chain[ci];
        size_t shift = (size_t)val;
        if (shift == 0) continue;
        size_t word_shift = shift >> 6;
        unsigned bit_shift = shift & 63;

        /* Copy current state to tmp. */
        memcpy(bitset_tmp, bitset, words * sizeof(uint64_t));

        /* Shift tmp left by 'shift' bits and OR into bitset (in-place). */
        if (word_shift >= words) continue;

        if (bit_shift == 0)
        {
            /* Simple word shift. */
            for (size_t i = words; i-- > word_shift; )
                bitset[i] |= bitset_tmp[i - word_shift];
        }
        else
        {
            /* Word + bit shift. */
            for (size_t i = words; i-- > word_shift + 1; )
            {
                uint64_t high = bitset_tmp[i - word_shift] << bit_shift;
                uint64_t low  = bitset_tmp[i - word_shift - 1] >> (64 - bit_shift);
                bitset[i] |= high | low;
            }
            /* Handle the first word shifted into itself. */
            bitset[word_shift] |= bitset_tmp[0] << bit_shift;
        }
    }

    size_t idx = (size_t)(target >> 6);
    unsigned offset = (unsigned)(target & 63);
    return ((bitset[idx] >> offset) & 1ULL) != 0;
}

/* ----------  Pseudoperfect totient test  ---------- */

/* Return true iff n is a pseudoperfect totient number. */
static bool is_pseudoperfect_totient(seq_t n)
{
    /* Build the iterated φ chain (excluding n itself). */
    seq_t chain[64];
    size_t len = 0;
    seq_t cur = calc_phi(n);
    while (true)
    {
        if (len >= sizeof(chain) / sizeof(chain[0])) break; /* safety */
        chain[len++] = cur;
        if (cur == 1) break;
        cur = calc_phi(cur);
    }
    return subset_sum_possible(n, chain, len);
}

/* ----------  Main API: sequence a(n)  ---------- */

/* Dynamic array of found pseudoperfect totient numbers. */
static seq_t *seq = NULL;
static size_t seq_size  = 0;
static size_t seq_cap   = 0;
static seq_t next_candidate = 3;  /* start testing from the smallest odd */

/* Return the nth pseudoperfect totient number (offset 1). */
seq_t a(idx_t n)
{
    /* Grow the cached list until we have at least n terms. */
    while (seq_size < (size_t)n)
    {
        seq_t cand = next_candidate;
        if (is_pseudoperfect_totient(cand))
        {
            if (seq_size == seq_cap)
            {
                size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
                ensure_capacity(new_cap, &seq_cap, (void **)&seq, sizeof(seq_t));
            }
            seq[seq_size++] = cand;
        }
        next_candidate += 2;   /* skip even numbers – all terms are odd */
    }
    return seq[n - 1];
}
