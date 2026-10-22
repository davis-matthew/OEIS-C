/*
 * A373492  Numbers k for which A059975(k) and A083345(k) are both multiples
 *          of 3, where A059975 is fully additive with a(p) = p-1, and A083345
 *          is the numerator of the fully additive function with a(p) = 1/p.
 * https://oeis.org/A373492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 4336, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 7745, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula:  a(n) = k where k is the n-th integer such that
   A059975(k) and A083345(k) are both multiples of 3.  */

/* The sequence is generated on demand by testing successive integers
   and storing those that satisfy the two conditions.  The two
   conditions are expressed directly in terms of the prime
   factorisation of k:

   1. Let e3 be the exponent of the prime 3 in k and
      E1 = sum_{p≡2 (mod 3)} exponent(p).
      Then A059975(k) ≡ E1 + 2⋅e3 (mod 3).  We require
      E1 + 2⋅e3 ≡ 0 (mod 3).

   2. Let N = ∑ e_i ⋅ (∏_{q≠p_i} q)   (q runs over the distinct primes of k).
      Let D = ∏_q q   (distinct primes).  Then
      - if 3∤k, we need N ≡ 0 (mod 3);
      - if 3|k, we need N ≡ 0 (mod 9)   (because the 3 factor
        that divides D cancels one factor of 3 from N).

   Both conditions are tested modulo 9, which is enough for the
   second because divisibility by 9 decides whether the reduced
   numerator is a multiple of 3.  The prime factorisation is
   obtained with the supplied helper
   `factor_u64(n, primes, exps, &cnt)`.  No other external
   library functions are used.  */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t *seq_vals = NULL;   /* sequence elements already found */
static size_t   seq_len  = 0;     /* count of stored elements         */
static idx_t    next_candidate = 1;   /* next integer to test */

/* Helper routine that grows a seq_t array to at least new_len elements.  */
static void grow_seq_t(seq_t **arr, size_t *cap, size_t new_len)
{
    if (*cap >= new_len) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < new_len) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Test whether k satisfies the two conditions for the A373492 sequence. */
static bool is_good_k(uint64_t k)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);

    /* Condition 1: A059975(k) divisible by 3 */
    unsigned e3 = 0;
    unsigned sum_E1 = 0;   /* sum of exponents for primes ≡ 2 (mod 3) */
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 3ULL) e3 = (unsigned)exps[i];
        if (primes[i] % 3ULL == 2ULL) sum_E1 += (unsigned)exps[i];
    }
    if ((sum_E1 + 2 * e3) % 3 != 0) return false;

    /* Condition 2: A083345(k) multiple of 3, evaluated via N modulo 9 */
    /* Compute D mod 9 and D/p_i mod 9 for each distinct prime. */
    unsigned prefix[65];
    unsigned suffix[65];
    prefix[0] = 1;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned m = (unsigned)(primes[i] % 9ULL);
        prefix[i + 1] = (prefix[i] * m) % 9;
    }
    suffix[cnt] = 1;
    for (size_t i = cnt; i-- > 0;) {
        unsigned m = (unsigned)(primes[i] % 9ULL);
        suffix[i] = (m * suffix[i + 1]) % 9;
    }

    unsigned N_mod9 = 0;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned e = (unsigned)exps[i];
        /* D / p_i modulo 9 is prefix[i] * suffix[i+1] mod 9 */
        unsigned D_over_p = (prefix[i] * suffix[i + 1]) % 9;
        N_mod9 = (N_mod9 + e * D_over_p) % 9;
    }

    bool has_three = (e3 > 0);
    if (has_three) {
        /* Need N ≡ 0 (mod 9) */
        return N_mod9 == 0;
    } else {
        /* Need N ≡ 0 (mod 3) */
        return (N_mod9 % 3) == 0;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, but safe */

    /* Extend the cached sequence until it contains at least n terms. */
    while (seq_len < (size_t)n) {
        if (is_good_k((uint64_t)next_candidate)) {
            /* Append the candidate to the array. */
            size_t new_len = seq_len + 1;
            size_t cap = seq_len > 0 ? (new_len > seq_len ? new_len : seq_len) : 0;
            /* Simple growth strategy: double capacity when needed. */
            if (cap < new_len) {
                cap = (cap == 0) ? 16 : (cap << 1);
                while (cap < new_len) cap <<= 1;
                seq_vals = (seq_t *)realloc(seq_vals, cap * sizeof(seq_t));
            }
            seq_vals[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq_vals[n - 1];
}
