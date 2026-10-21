/*
 * A279814  Numbers n such that the average of the squares of the proper
 *          divisors of n is an integer.
 * https://oeis.org/A279814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 724, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A279814
   (Numbers n such that the number of proper divisors of n
    divides the sum of squares of the proper divisors). */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Memoised list of sequence values – allows out‑of‑sight queries. */
static seq_t *s_seq          = NULL;   /* array of terms */
static size_t s_len          = 0;      /* number of terms stored */
static size_t s_cap          = 0;      /* array capacity */
static uint64_t s_last_checked = 1;    /* last integer examined */

/* Ensure that s_seq has space for at least *needed* elements. */
static void s_grow(size_t needed)
{
    if (s_cap < needed) {
        size_t new_cap = s_cap ? s_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(s_seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                 /* allocation failure */
        s_seq = tmp;
        s_cap = new_cap;
    }
}

/* Return the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n==0, defensive */

    while (s_len < (size_t)n) {
        uint64_t m = ++s_last_checked;     /* next integer to test */

        /* Factor m.  factor_u64 stores factors in `primes`
           and their exponents in `exps`. */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t   cnt = 0;
        factor_u64(m, primes, exps, &cnt);

        /* tau = number of divisors of m (including 1 and m). */
        seq_t tau = 1;
        /* sigma2 = Σ d² for all divisors d of m. */
        seq_t sigma2 = 1;

        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            size_t   e = exps[i];
            tau *= (seq_t)(e + 1);

            /* Compute 1 + p² + … + p²ⁱ for i=1..e. */
            seq_t term = 1;
            seq_t pe2  = 1;                 /* current p¹²ʲ */
            for (size_t j = 1; j <= e; ++j) {
                pe2 *= (seq_t)p * (seq_t)p;
                term += pe2;
            }
            sigma2 *= term;
        }

        /* Sum of squares of proper divisors = sigma2 - m². */
        seq_t m_sq = (seq_t)m * (seq_t)m;
        seq_t S    = sigma2 - m_sq;

        /* Number of proper divisors = tau - 1. */
        seq_t k = tau - 1;
        if (k != 0 && S % k == 0) {
            s_grow(s_len + 1);
            s_seq[s_len++] = (seq_t)m;
        }
    }

    return s_seq[n - 1];
}
