/*
 * A304408  If n = Product (p_j^k_j) then a(n) = Product ((p_j - 1)*(k_j +
 *          1)).
 * https://oeis.org/A304408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^k‖n} (p-1)*(k+1) (OEIS A304408)
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Simple trial‑division factorisation for 64‑bit numbers.
 * Stores primes in 'pr', exponents in 'ex', and updates '*cnt'.
 * It is used only by function a() and is thus declared static. */
static void
factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    *cnt = 0;
    if (n < 2)                      /* 1 has no prime factors */
        return;

    /* factor out the 2's */
    if (n % 2 == 0) {
        size_t e = 0;
        while ((n & 1ULL) == 0) {   /* faster than n % 2 */
            n >>= 1;
            ++e;
        }
        pr[*cnt] = 2;
        ex[*cnt] = e;
        ++(*cnt);
    }

    /* test odd divisors */
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) {
            size_t e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            pr[*cnt] = d;
            ex[*cnt] = e;
            ++(*cnt);
        }
    }

    /* remaining n is a prime > 2 */
    if (n > 1) {
        pr[*cnt] = n;
        ex[*cnt] = 1;
        ++(*cnt);
    }
}

/* Computes a(n) for OEIS sequence A304408.
 * Works for any idx_t ≥ 1.  The implementation is O(√n). */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term1 = (seq_t)(primes[i] - 1); /* p - 1 */
        seq_t term2 = (seq_t)(exps[i] + 1);   /* k + 1 */
        result *= term1 * term2;
    }
    return result;
}
