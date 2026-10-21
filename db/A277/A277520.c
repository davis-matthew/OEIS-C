/*
 * A277520  Denominator of 3F2([3*n, -n, n+1],[2*n+1, n+1/2], 1).
 * https://oeis.org/A277520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of 3F2([3*n, -n, n+1],[2*n+1, n+1/2], 1);
 *            Uses the closed forms: s(2k) = ( ((4k)!*(3k)!)/((6k)!*k!) )^2
 *            s(2k+1) = -1/3 * ( ((4k+1)!*(3k)!)/((6k+1)!*k!) )^2 Hence a(2k)
 *            = Π p^{2*(e_den-e_num)} , a(2k+1) = 3 * Π
 *            p^{2*max(0,e_den-e_num)} where e_den,e_num are the exponents of
 *            prim...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"

static idx_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_list) return;
    /* 10 000 needs primes up to 30001 (≈ 6*n+1), use 60000 to be safe */
    const idx_t LIMIT = 60000;
    prime_list = (idx_t *)malloc((LIMIT + 1) * sizeof(idx_t));
    if (!prime_list) return;
    prime_count = 0;
    for (idx_t p = 2; p <= LIMIT; ++p) {
        bool isprime = true;
        for (size_t j = 0; j < prime_count &&
                           prime_list[j] * prime_list[j] <= p; ++j) {
            if (p % prime_list[j] == 0) { isprime = false; break; }
        }
        if (isprime) prime_list[prime_count++] = p;
    }
}

/* exponent of prime p in n! (Legendre's theorem) */
static idx_t exp_factorial(idx_t n, idx_t p)
{
    idx_t e = 0;
    for (idx_t d = p; d <= n; d *= p) e += n / d;
    return e;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;      /* a(0)=1 */

    init_primes();

    const idx_t k = n / 2;            /* for both even and odd: k=floor(n/2) */
    const bool odd = (n & 1);

    /* Choose factorial bounds */
    const idx_t B1 = odd ? (4 * k + 1) : (4 * k);   /* (4k+1)! or (4k)!   */
    const idx_t B2 = 3 * k;                         /* (3k)!                */
    const idx_t D1 = odd ? (6 * k + 1) : (6 * k);   /* (6k+1)! or (6k)!   */
    const idx_t D2 = k;                             /* (k)!                 */

    const idx_t limit = D1;                         /* largest needed prime */
    seq_t res = 1;

    /* For every prime <= limit compute exponent difference */
    for (size_t i = 0; i < prime_count && prime_list[i] <= limit; ++i) {
        idx_t p = prime_list[i];
        idx_t e_num = exp_factorial(B1, p) + exp_factorial(B2, p);
        idx_t e_den = exp_factorial(D1, p) + exp_factorial(D2, p);
        if (e_den <= e_num) continue;              /* no factor in denominator */
        res *= pow_seq((seq_t)p, 2 * (e_den - e_num));
    }

    /* For odd n multiply by 3 only if the numerator of R has no factor 3 */
    if (odd) {
        idx_t e_num3 = exp_factorial(B1, 3) + exp_factorial(B2, 3);
        idx_t e_den3 = exp_factorial(D1, 3) + exp_factorial(D2, 3);
        if (e_num3 <= e_den3) res *= (seq_t)3;
    }

    return res;
}
