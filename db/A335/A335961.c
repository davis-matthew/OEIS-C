/*
 * A335961  Alternating factorions: Numbers m such that m = S_af(m) =
 *          af(d_1)+af(d_2)+...+af(d_k) where d_1 d_2 ... d_n is the decimal
 *          expansion of m and af(m) = m!-(m-1)!+(m-2)!+...1! (alternating
 *          factorial) with af(0) = 0 (base 10).
 * https://oeis.org/A335961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A335961: numbers m such that S_af(m) = m,
   where S_af(m) = ∑ af(d_i) for the decimal digits d_i of m
   and af(d) = d! – (d-1)! + (d-2)! – … + (−1)^{d+1}·1!  (af(0)=0) */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static bool prepared = false;
static seq_t solutions[10];
static idx_t solcnt = 0;

#define MAX_M 2000000ULL   /* search upper bound (covers first 5 terms) */

static void compute_solutions(void)
{
    /* precompute factorials up to 9 (fits in seq_t) */
    seq_t fact[10];
    fact[0] = (seq_t)1;
    for (uint64_t i = 1; i < 10; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* precompute alternating factorials for digits 0..9 */
    seq_t af[10];
    af[0] = (seq_t)0;
    for (int d = 1; d < 10; ++d) {
        seq_t sum = (seq_t)0;
        bool add = true;
        for (int k = d; k >= 1; --k) {
            if (add)
                sum += fact[k];
            else
                sum -= fact[k];
            add = !add;
        }
        af[d] = sum;
    }

    /* search for solutions */
    for (idx_t m = 0; m <= MAX_M; ++m) {
        seq_t val = (seq_t)0;
        seq_t tmp = (seq_t)m;
        if (tmp == (seq_t)0)
            val = af[0];
        else {
            while (tmp > (seq_t)0) {
                int digit = (int)(tmp % (seq_t)10);
                val += af[digit];
                tmp /= (seq_t)10;
            }
        }
        if (val == (seq_t)m) {
            if (solcnt < sizeof(solutions) / sizeof(solutions[0]))
                solutions[solcnt++] = (seq_t)m;
        }
    }
    prepared = true;
}

seq_t a(idx_t n)
{
    if (!prepared)
        compute_solutions();
    if (n == 0 || n > solcnt)
        return (seq_t)0;
    return solutions[n - 1];
}
