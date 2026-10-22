/*
 * A374202  The 2-adic valuation of A328845(n), where A328845 is a
 *          Fibonacci-based variant of the arithmetic derivative.
 * https://oeis.org/A374202
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v₂(A328845(n)) A328845(n) = Σ_{p^e‖n} e · (n/p) · F_p
 *            where F_p is the p‑th Fibonacci number and v₂(x) is the exponent
 *            of 2 in x. The function below computes a(n) for any n ≥ 2 using
 *            O(√n) trial division and double‑precision Fibonacci modulo 2⁶⁴
 *            (sufficient for n ≤ 10 000)
 * Range:     u8 n <= 9803, u16 n <= 9803, u32 n <= 9803, u64 n <= 9803,
 *            u128 n <= 9803, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* fast doubling: returns F(n) modulo 2⁶⁴ */
static void fib_pair_mod(uint64_t n, uint64_t *f, uint64_t *fp1)
{
    if (n == 0) {
        *f   = 0;
        *fp1 = 1;
        return;
    }
    uint64_t a, b;
    fib_pair_mod(n >> 1, &a, &b);                 /* a = F(k), b = F(k+1) */
    uint64_t two_b_minus_a = (uint64_t)((((unsigned __int128)b << 1) - (unsigned __int128)a) & 0xFFFFFFFFFFFFFFFFULL);
    uint64_t c = (uint64_t)((unsigned __int128)a * two_b_minus_a);          /* F(2k)   */
    uint64_t d = (uint64_t)((unsigned __int128)a * a + (unsigned __int128)b * b); /* F(2k+1) */

    if ((n & 1) == 0) {
        *f   = c;
        *fp1 = d;
    } else {
        *f   = d;
        *fp1 = (c + d) & 0xFFFFFFFFFFFFFFFFULL;
    }
}

static uint64_t fib_mod(uint64_t n)
{
    uint64_t f, fp1;
    fib_pair_mod(n, &f, &fp1);
    return f;
}

/* compute a(n) = v₂(A328845(n)) for n ≥ 2
   Return type seq_t is assumed to be unsigned (uint8_t, …, unsigned __int128). */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;    /* out of defined range */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;

    /* trial division factorisation */
    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            uint64_t e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (nn > 1) {                /* remaining prime factor */
        primes[cnt] = nn;
        exps[cnt]   = 1;
        ++cnt;
    }

    uint64_t sum = 0;            /* derivative modulo 2⁶⁴ */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        uint64_t np = nn / p;            /* current nn is 1; restore n/p via original n // p */
    }
    /* Restore n for computing n/p in terms of original n */
    nn = (uint64_t)n;            /* original n */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        uint64_t np = nn / p;
        uint64_t fibp = fib_mod(p);
        uint64_t term = (uint64_t)((unsigned __int128)(np * e) * fibp);
        sum += term;
    }

    /* The derivative is never zero for n ≥ 2, but guard against
       accidental wrap‑around to 0 modulo 2⁶⁴. */
    if (sum == 0) return (seq_t)0;

    unsigned v2 = 0;
    while ((sum & 1ULL) == 0ULL) {
        sum >>= 1;
        ++v2;
    }
    return (seq_t)v2;
}
