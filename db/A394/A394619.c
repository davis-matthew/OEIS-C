/*
 * A394619  Number of divisors d | k such that d is neither squarefree nor
 *          squareful (in A332785) and k = A126706(n).
 * https://oeis.org/A394619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000005(k) – A005361(k) – 2^A001221(k) + 1 with k =
 *            A126706(n) (the n‑th integer that has at least two distinct
 *            prime factors and at least one squared prime factor)
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"  /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */
#include "../utils/pow2.h"        /* pow2(idx_t exp) -> seq_t */

/* --- cache of the sequence A126706 ----------------------------------- */
static seq_t *alist      = NULL;   /* list of A126706 values (seq_t)        */
static size_t alist_cap  = 0;
static size_t alist_len  = 0;
static idx_t  last_checked = 1;    /* last integer examined for the list */

/* append a seq_t value to alist, growing capacity as needed */
static void alist_append(seq_t v)
{
    if (alist_len >= alist_cap) {
        size_t new_cap = alist_cap ? alist_cap << 1 : 1024;
        seq_t *new_arr = (seq_t *)realloc(alist, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        alist = new_arr;
        alist_cap = new_cap;
    }
    alist[alist_len++] = v;
}

/* test whether n satisfies the A126706 condition:
 *   • at least two distinct prime factors,
 *   • at least one prime factor of exponent ≥ 2                 */
static bool is_a126706_candidate(idx_t n)
{
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);   /* fits in uint64_t */
    if (cnt < 2) return false;
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] >= 2) return true;
    return false;
}

/*  main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for accidental offset‑0 call */

    /* generate A126706 values until we have n terms */
    while (alist_len < (size_t)n) {
        ++last_checked;
        if (is_a126706_candidate(last_checked))
            alist_append((seq_t)last_checked);
    }

    /* retrieve the required k = A126706(n) */
    seq_t k = alist[(size_t)n - 1];

    /* factor k (fit comfortably in uint64_t) */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)k, primes, exps, &cnt);  /* cnt = ω(k) */

    seq_t tau   = 1;              /* τ(k) = number of divisors */
    seq_t prodE = 1;              /* product of exponents in prime factorization */
    for (size_t i = 0; i < cnt; ++i) {
        tau   *= (seq_t)(exps[i] + 1);
        prodE *= (seq_t)exps[i];
    }

    /* 2^ω(k)  computed with the utility pow2(idx_t) */
    seq_t two_pow_omega = pow2((idx_t)cnt);

    /* a(n) = τ(k) – prodE – 2^ω(k) + 1 */
    seq_t result = tau - prodE - two_pow_omega + 1;
    return result;
}
