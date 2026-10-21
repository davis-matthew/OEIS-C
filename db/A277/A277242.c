/*
 * A277242  Number of factorizations of m^n into exactly six factors, where m
 *          is a product of two distinct primes.
 * https://oeis.org/A277242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 20, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   compute_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A277242
 *  Number of factorizations of (p*q)^n into exactly six unordered factors,
 *  where p and q are distinct primes.
 *
 *  The program applies Burnside's lemma to the action of the symmetric
 *  group S6 on ordered 6‑tuples of exponents.  For a permutation σ
 *  with cycle lengths ℓ1,…,ℓt the number of ordered tuples fixed by σ
 *  equals g(n,ℓ)^2, where g(n,ℓ) is the number of non‑negative integer
 *  solutions to Σ ℓi·ai = n.  Summing over the 11 conjugacy classes of S6
 *  and dividing by |S6| = 720 yields a(n).  g(n,ℓ) is computed by a
 *  simple coin‑change DP.
 *
 *  This implementation is O(|S6| * n * t) ≈ O( n ) per call and works for
 *  arbitrary‑precision seq_t as well as built‑in unsigned integer types.
 */

#include <stdlib.h>
#include "../utils/compute_g.h"

/* Structure describing a conjugacy class of S6 */
typedef struct {
    idx_t len[6];   /* cycle lengths (unused entries are 0) */
    idx_t cnt;      /* number of cycles in the cycle description */
    seq_t perm;     /* number of permutations in this conjugacy class */
} CycleInfo;

/* Conjugacy classes of S6, in order of cycle type:
 * [6], [5,1], [4,2], [4,1,1], [3,3], [3,2,1], [3,1,1,1],
 * [2,2,2], [2,2,1,1], [2,1,1,1,1], [1,1,1,1,1,1]   */
static const CycleInfo cycle_types[] = {
    { {6}, 1, 120 },      /* [6]            */
    { {5,1}, 2, 144 },    /* [5,1]          */
    { {4,2}, 2, 90 },     /* [4,2]          */
    { {4,1,1}, 3, 90 },   /* [4,1,1]        */
    { {3,3}, 2, 40 },     /* [3,3]          */
    { {3,2,1}, 3, 120 },  /* [3,2,1]        */
    { {3,1,1,1}, 4, 40 }, /* [3,1,1,1]      */
    { {2,2,2}, 3, 15 },   /* [2,2,2]        */
    { {2,2,1,1}, 4, 45 }, /* [2,2,1,1]      */
    { {2,1,1,1,1}, 5, 15 }, /* [2,1,1,1,1]  */
    { {1,1,1,1,1,1}, 6, 1 } /* [1,1,1,1,1,1]  */
};

static const size_t NUM_TYPES = sizeof(cycle_types) / sizeof(cycle_types[0]);

seq_t a(idx_t n)
{
    seq_t total = 0;   /* accumulate sum of g^2 * perm over all classes */

    for (size_t k = 0; k < NUM_TYPES; ++k) {
        const CycleInfo *info = &cycle_types[k];
        seq_t g = compute_g(n, info->len, info->cnt);
        total += g * g * info->perm;
    }

    /* Divide by |S6| = 720 to get the number of unordered factorizations. */
    return total / (seq_t)720;
}
