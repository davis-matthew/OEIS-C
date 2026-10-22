/*
 * A380524  a(n) = 1 if n is a squarefree and for all factorizations of n as
 *          x*y, the sum (x * y') + (x' * y) is carryfree when the addition is
 *          done in the primorial base, otherwise 0. Here u' stands for
 *          A003415(u), the arithmetic derivative of u.
 * https://oeis.org/A380524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4168, u16 n <= 9075, u32 n <= 9075, u64 n <= 9075,
 *            u128 n <= 9075, big any n
 * Helpers:   build_primes_upto, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 iff n is squarefree and for every factorization n = x*y, the
 * sum (x*y') + (x'*y) is carry‑free in the primorial base, else 0.
 * (Here u' denotes the arithmetic derivative, A003415(u).)
 */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_squarefree.h"

/* cache of primes used for primorial base and for factoring */
static uint64_t *g_primes = NULL;
static size_t   g_prime_cnt = 0;

/* initialise the prime list once */
static void init_primes(void)
{
    if (g_primes != NULL)
        return;
    /* enough primes so that the primorial weight grows beyond 10^12
       (more than sufficient for n<=10000). */
    const idx_t LIMIT = 2000;
    build_primes_upto(LIMIT, (uint64_t **)&g_primes, &g_prime_cnt);
}

/* arithmetic derivative of a non‑negative integer */
static seq_t derivative(seq_t n)
{
    if (n < (seq_t)2)
        return (seq_t)0;

    init_primes();

    uint64_t u = (uint64_t)n;          /* we work with uint64_t internally */
    seq_t    der = (seq_t)0;

    for (size_t i = 0; i < g_prime_cnt && (uint64_t)g_primes[i] * (uint64_t)g_primes[i] <= u; ++i) {
        uint64_t p = (uint64_t)g_primes[i];
        if (u % p == 0) {
            uint64_t exp = 0;
            while (u % p == 0) {
                u /= p;
                ++exp;
            }
            der += (seq_t)exp * n / p;
        }
    }
    if (u > 1) { /* remaining prime factor */
        der += (seq_t)1 * n / u;
    }
    return der;
}

/* check whether the addition of X and Y is carry‑free in the primorial base */
static bool carryfree(seq_t X, seq_t Y)
{
    seq_t maxv = X > Y ? X : Y;
    if (maxv == (seq_t)0)
        return true;

    init_primes();

    seq_t weight = (seq_t)1;   /* product of previous primes */
    size_t idx = 0;            /* current prime index in g_primes */

    while (weight <= maxv) {
        seq_t base = g_primes[idx];          /* radicand for this digit */
        seq_t digitX = (X / weight) % base;
        seq_t digitY = (Y / weight) % base;
        if (digitX + digitY >= base)
            return false;

        /* avoid overflow when multiplying weight by base */
        if (weight > maxv / base)
            break;
        weight *= base;
        ++idx;
    }
    return true;
}

/* public sequence entry */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if (!is_squarefree(sn))
        return (seq_t)0;

    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;
        uint64_t x = d;
        uint64_t y = n / d;

        seq_t derX = derivative((seq_t)x);
        seq_t derY = derivative((seq_t)y);

        seq_t X = (seq_t)x * derY;  /* x * y' */
        seq_t Y = derX * (seq_t)y;  /* x' * y */

        if (!carryfree(X, Y))
            return (seq_t)0;
    }
    return (seq_t)1;
}
