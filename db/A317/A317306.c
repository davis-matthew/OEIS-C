/*
 * A317306  Powers of 2 and even perfect numbers.
 * https://oeis.org/A317306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 37, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th smallest element of the set
   { 2^k | k ≥ 0 } ∪ { 2^(p‑1)(2^p‑1) | 2^p‑1 is Mersenne prime }.
   The sequence lists powers of two and even perfect numbers in
   ascending order. */

#include <stdbool.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */
#include "../utils/seq_mul.h"/* seq_t seq_mul(seq_t a, seq_t b); */

static const idx_t perfect_exps[] = {
    2, 3, 5, 7, 13, 17, 19, 31, 61, 89, 107, 127, 521
};                      /* exponents p for known Mersenne primes */

static seq_t perfects[sizeof(perfect_exps)/sizeof(perfect_exps[0])];
static size_t perfect_count = 0;
static bool perfects_initialized = false;

static void init_perfects(void)
{
    if (perfects_initialized) return;
    size_t n = sizeof(perfect_exps)/sizeof(perfect_exps[0]);

    for (size_t i = 0; i < n; ++i) {
        idx_t p = perfect_exps[i];
        seq_t two_p      = pow2(p);          /* 2^p          */
        seq_t two_p_minus1 = two_p - (seq_t)1; /* 2^p – 1      */
        seq_t two_p_minus1_power = pow2(p-1); /* 2^(p‑1)      */
        perfects[i] = seq_mul(two_p_minus1_power, two_p_minus1);
    }
    perfect_count = n;
    perfects_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; keep safe for n==0 */

    init_perfects();

    idx_t power_exp = 0;          /* exponent for the current power of two */
    seq_t cur_power = pow2(0);    /* 2^0 = 1 */
    size_t pidx = 0;              /* index into perfects array          */
    idx_t count = 0;              /* how many terms have been returned */

    while (true) {
        seq_t next_perfect = (pidx < perfect_count) ? perfects[pidx] : 0;
        bool take_power = false;

        if (pidx >= perfect_count) {
            take_power = true;             /* no more perfect numbers */
        } else if (cur_power <= next_perfect) {
            take_power = true;             /* power comes first */
        } else {
            take_power = false;            /* perfect number comes first */
        }

        if (take_power) {
            ++count;
            if ((idx_t)count == n) return cur_power;
            ++power_exp;
            cur_power = pow2(power_exp);
        } else {
            ++count;
            if ((idx_t)count == n) return next_perfect;
            ++pidx;
        }
    }
}
