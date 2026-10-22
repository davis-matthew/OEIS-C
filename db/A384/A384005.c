/*
 * A384005  Number of ways to choose disjoint strict integer partitions, one
 *          of each conjugate prime index of n.
 * https://oeis.org/A384005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A383706(A122111(n))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the OEIS sequence A384005.
   It factors n, builds the multiset of prime indices, finds its  
   conjugate partition λ', and counts the ways to choose for each
   part of λ' a strict partition whose parts are all distinct
   across all chosen partitions.  The recursion depth is at most
   the number of distinct prime indices (≤ bigOmega(n)), which is
   small for n ≤ 10 000. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Provided helper functions */
#include "../utils/factor_u64.h"      /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/compute_pi.h"     /* seq_t compute_pi(idx_t n); */

/* Small vector of masks for strict partitions of a given size */
typedef struct {
    uint32_t *masks;
    size_t   len;
} mask_vec_t;

/* Generate all strict partitions of value 'n' (parts strictly decreasing).
   The mask represents the set of part numbers used (bit k-1 indicates part k). */
static void gen_strict_partitions(int n, int max_part, uint32_t mask, mask_vec_t *out)
{
    if (n == 0) {
        out->masks = (uint32_t *)realloc(out->masks, (out->len + 1) * sizeof(uint32_t));
        out->masks[out->len++] = mask;
        return;
    }
    int start = (n < max_part) ? n : max_part;
    for (int k = start; k >= 1; --k) {
        gen_strict_partitions(n - k, k - 1, mask | (1U << (k - 1)), out);
    }
}

/* Recursive backtracking: count partitions for parts from index 'pos',
   with 'used' mask of already taken part numbers. */
static seq_t dfs_partitions(const int *parts, size_t pos, size_t len,
                            const mask_vec_t *vecs, uint32_t used)
{
    if (pos == len)
        return (seq_t)1;
    seq_t cnt = (seq_t)0;
    const mask_vec_t *vec = &vecs[parts[pos]];
    for (size_t i = 0; i < vec->len; ++i) {
        uint32_t m = vec->masks[i];
        if ((used & m) == 0)
            cnt += dfs_partitions(parts, pos + 1, len, vecs, used | m);
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* n = 1 is a special case */
    if (n <= 1)
        return (seq_t)1;

    /* 1. Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* 2. Build frequency of prime indices */
    struct { idx_t idx; idx_t cnt; } freq[64];
    size_t freqcnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        idx_t idx = (idx_t)compute_pi(p);          /* index of prime p */
        /* accumulate multiplicity exp[i] */
        bool found = false;
        for (size_t j = 0; j < freqcnt; ++j) {
            if (freq[j].idx == idx) {
                freq[j].cnt += exps[i];
                found = true;
                break;
            }
        }
        if (!found) {
            freq[freqcnt].idx = idx;
            freq[freqcnt].cnt = exps[i];
            ++freqcnt;
        }
    }

    /* 3. Determine maxIndex and bigOmega */
    idx_t maxIndex = 0;
    idx_t bigOmega = 0;
    for (size_t i = 0; i < freqcnt; ++i) {
        if (freq[i].idx > maxIndex)
            maxIndex = freq[i].idx;
        bigOmega += freq[i].cnt;
    }
    if (bigOmega == 0)
        return (seq_t)1;   /* n was 1, already handled */

    /* 4. Build conjugate partition λ' : parts[j] = number of indices >= (j+1) */
    size_t parts_len = (size_t)maxIndex;
    int *parts = (int *)malloc(parts_len * sizeof(int));
    idx_t remaining = bigOmega;
    for (idx_t j = 1; j <= maxIndex; ++j) {
        parts[(size_t)(j - 1)] = (int)remaining;
        /* subtract frequency of index j if present */
        for (size_t t = 0; t < freqcnt; ++t) {
            if (freq[t].idx == j) {
                remaining -= freq[t].cnt;
                break;
            }
        }
    }

    /* 5. Precompute strict partitions for all sizes up to bigOmega */
    size_t max_s = (size_t)bigOmega;   /* largest part size is the first part */
    mask_vec_t *vecs = (mask_vec_t *)malloc((max_s + 1) * sizeof(mask_vec_t));
    for (size_t s = 0; s <= max_s; ++s) {
        vecs[s].masks = NULL;
        vecs[s].len   = 0;
    }
    for (size_t s = 1; s <= max_s; ++s) {
        gen_strict_partitions((int)s, (int)s, 0U, &vecs[s]);
    }

    /* 6. Backtracking to count disjoint strict partitions */
    seq_t result = dfs_partitions(parts, 0, parts_len, vecs, 0U);

    /* 7. Clean up */
    for (size_t s = 1; s <= max_s; ++s)
        free(vecs[s].masks);
    free(vecs);
    free(parts);

    return result;
}
