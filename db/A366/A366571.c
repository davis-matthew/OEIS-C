/*
 * A366571  a(n) = denominator(Bernoulli(n, x)) / denominator(Bernoulli'(n,
 *          x)).
 * https://oeis.org/A366571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denom(Bernoulli(n,x)) / denom(Bernoulli'(n,x))
 *            (denominator of the nth Bernoulli polynomial divided by the
 *            denominator of its derivative). The implementation builds the
 *            primes up to n+1, tests whether a prime p contributes to the
 *            denominator by checking whether there exists a k with (p‑1)...
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binom_not_divisible, build_primes_upto, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_gcd.h"
#include "../utils/binom_not_divisible.h"

static uint64_t *prime_list   = NULL;
static size_t   prime_cnt    = 0;
static idx_t    prime_limit  = 0;

static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    uint64_t *list;
    size_t cnt;
    build_primes_upto(limit, &list, &cnt);
    prime_list  = list;
    prime_cnt   = cnt;
    prime_limit = limit;
}

/* Denominator of the nth Bernoulli polynomial.  The product
   is taken over all odd primes and p=2 if they satisfy the
   non‑divisibility test described above. */
static seq_t denom_of_bpoly(idx_t n)
{
    seq_t denom = 1;
    if (n == 0)
        return 1;
    ensure_primes_upto(n + 1);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > n + 1)
            break;          /* primes beyond n+1 cannot contribute */
        uint64_t step = p - 1;   /* skip p==2? step>=1 always */
        if (step == 0)
            continue;        /* defensive, p cannot be 1 */
        bool contrib = false;
        for (idx_t k = step; k <= n; k += step) {
            if (binom_not_divisible(n, k, p)) {
                contrib = true;
                break;
            }
        }
        if (contrib)
            denom *= (seq_t)p;
    }
    return denom;
}

/* Public entry point of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                 /* a(0) = 1 by definition */
    seq_t Pn      = denom_of_bpoly(n);
    seq_t Pnm1    = denom_of_bpoly(n - 1);
    seq_t g       = seq_gcd((seq_t)n, Pnm1);   /* gcd between n and denominator of B_{n-1} */
    return Pn * g / Pnm1;
}
