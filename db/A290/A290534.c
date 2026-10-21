/*
 * A290534  Denominator of 2*n*(2*n+1) B_{2*n}, where B_n are the Bernoulli
 *          numbers.
 * https://oeis.org/A290534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 125, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A290534 – Denominator of 2*n*(2*n+1)*B_{2*n}
 *  B_{2n} is the Bernoulli number.
 *  Von Staudt–Clausen theorem: the denominator of B_{2n} is the
 *  product of all primes p with (p–1) | 2n.
 *  Multiply by 2*n*(2*n+1); any common prime factor cancels out.
 *
 *  a(0) = 1   (special case – B_0 = 1)
 *
 *  For n >= 1:
 *      denom = ∏_{p ≤ 2n+1 , (p-1)|2n,  p ∤ 2n and p ∤ (2n+1) } p
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_N     10000
#define MAX_P     (2*MAX_N+1)          /* 20001 */

/* ------------------------------------------------------------------ */
/* Prime sieve – computed once, on first call                         */
/* ------------------------------------------------------------------ */
static bool ___primes_init_done = false;
static uint32_t ___prime_list[MAX_P/10 + 10]; /* enough for primes <= MAX_P */
static size_t     ___prime_count = 0;

static void ___init_primes(void)
{
    if (___primes_init_done) return;
    bool sieve[MAX_P + 1];
    for (size_t i = 0; i <= MAX_P; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (uint32_t i = 2; i * i <= MAX_P; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= MAX_P; j += i) sieve[j] = false;
        }
    }
    for (uint32_t i = 2; i <= MAX_P; ++i) {
        if (sieve[i]) ___prime_list[___prime_count++] = i;
    }
    ___primes_init_done = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle offset.  B_0 = 1. */
    if (n == 0) return (seq_t)1;

    ___init_primes();

    seq_t denom = (seq_t)1;
    uint64_t two_n          = 2 * n;         /* 2*n  */
    uint64_t two_n_plus_one = two_n + 1;     /* 2*n+1 */

    for (size_t i = 0; i < ___prime_count && 
                       ___prime_list[i] <= two_n_plus_one; ++i) {
        uint32_t p = ___prime_list[i];
        if ((two_n % (p - 1)) == 0) {                   /* p contributes */
            /* Cancel if p divides the numerator */
            if ((two_n % p) == 0 || (two_n_plus_one % p) == 0)
                continue;
            denom = denom * (seq_t)p;
        }
    }
    return denom;
}
