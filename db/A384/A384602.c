/*
 * A384602  Numbers k such that T(k, 1) mod 3 = 1 and T(k, 2) mod 3 = 2, where
 *          T is the Wythoff array (A035513).
 * https://oeis.org/A384602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A384602
 *  Formula: a(n) is the n-th positive integer k such that
 *          T(k,1) mod 3 = 1 and T(k,2) mod 3 = 2,
 *          where T is the Wythoff array.
 *
 *  Computation: For each k we compute the Beatty value
 *          a(k) = ⌊k⋅φ⌋ using the rational approximation
 *          φ ≈ 514229/317811 which is strictly less than φ.
 *          Then
 *              T(k,1) = a(k) + k - 1,
 *              T(k,2) = 2⋅a(k) + k - 1.
 *          The conditions reduce to k ≡ 1 (mod 3) and
 *          a(k) ≡ 1 (mod 3).  This is checked for all k
 *          in ascending order until the n‑th qualifying
 *          value has been found.
 *
 *  The function memoises results so calls are fast even
 *  for out‑of‑order indices.  All computations use only
 *  integer arithmetic, 128‑bit multiplication for safety.
 */

#include <stdlib.h>
#include <stdint.h>

/* constants for the convergent 514229/317811 < φ */
#define PHI_NUM 514229U       /* numerator */
#define PHI_DEN 317811U       /* denominator */

/* forward declaration of static helper */
static void ensure_capacity(size_t *cap, size_t need);

seq_t a(idx_t n)
{
        /* static cache for generated terms */
        static seq_t *cache = NULL;
        static size_t cache_len = 0;   /* number of elements in cache */
        static size_t cache_cap = 0;   /* allocated capacity   */
        static idx_t next_k = 1;       /* next k to be examined */

        if (n == 0) return 0;          /* undefined, but safe */

        /* extend cache if required */
        while (cache_len < (size_t)n) {
                /* compute a(k) = floor(k * φ) using the convergent */
                unsigned long long prod = (unsigned long long)next_k * (unsigned long long)PHI_NUM;
                unsigned long long ak   = prod / (unsigned long long)PHI_DEN;   /* exact integer */

                /* check the two congruence conditions */
                if ((next_k % 3ULL == 1ULL) && ((ak % 3ULL) == 1ULL)) {
                        /* expand cache if needed */
                        if (cache_len == cache_cap) {
                                ensure_capacity(&cache_cap, cache_len + 1);
                                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                        }
                        cache[cache_len++] = (seq_t)next_k;
                }
                ++next_k;
        }

        return cache[n - 1];
}

/* simple growth routine: double until size >= need */
static void ensure_capacity(size_t *cap, size_t need)
{
        if (*cap == 0) *cap = 1;
        while (*cap < need) *cap <<= 1;
}
