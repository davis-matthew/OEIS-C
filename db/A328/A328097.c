/*
 * A328097  If A327928(n) is zero, a(n) = A003415(n), otherwise a(n) =
 *          A327859(n) = A276086(A003415(n)).
 * https://oeis.org/A328097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(n) if A327928(n)=0, else A276086(A003415(n));
 *            A003415 is the arithmetic derivative n' = Sum_{p|n} v_p(n)*n/p,
 *            A327928(n) counts primes p with p^p | n', and A276086(m) = Prod
 *            prime(k+1)^d_k, where d_k are the primorial-base digits of m
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 255, u64 n <= 1728,
 *            u128 n <= 1728, big any n
 * Validated: standard, pass, 97 terms through n = 1703 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool ad_is_prime_small(uint64_t p)
{
    if (p < 2) return false;
    for (uint64_t q = 2; q * q <= p; q++)
        if (p % q == 0) return false;
    return true;
}

static seq_t ad_derivative(idx_t n)
{
    if (n < 2) return 0;
    seq_t res = 0;
    uint64_t rem = (uint64_t)n;
    uint64_t N = (uint64_t)n;
    for (uint64_t p = 2; p * p <= rem; p++) {
        if (rem % p == 0) {
            unsigned e = 0;
            while (rem % p == 0) { rem /= p; e++; }
            res += (seq_t)(N / p) * (seq_t)e;
        }
    }
    if (rem > 1) res += (seq_t)(N / rem);
    return res;
}

/* does p^p divide d (d > 0)? */
static int ad_pp_divides(uint64_t p, seq_t d, bool *too_big)
{
    seq_t pw = 1;
    *too_big = false;
    for (uint64_t i = 0; i < p; i++) {
        if (pw > d / (seq_t)p) { *too_big = true; return 0; }
        pw *= (seq_t)p;
    }
    if (pw > d) { *too_big = true; return 0; }
    return (d % pw) == 0;
}

static seq_t ad_primorial_exp(seq_t m)
{
    seq_t result = 1;
    uint64_t p = 2;
    while (m != 0) {
        while (!ad_is_prime_small(p)) p++;
        seq_t digit = m % (seq_t)p;
        long long k = (long long)digit;
        for (long long i = 0; i < k; i++) result *= (seq_t)p;
        m /= (seq_t)p;
        p++;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t d = ad_derivative(n);
    if (d == 0) return 0;
    int cnt = 0;
    for (uint64_t p = 2; ; p++) {
        bool big;
        int ok = ad_pp_divides(p, d, &big);
        if (big) break;
        if (ok && ad_is_prime_small(p)) cnt++;
    }
    if (cnt == 0) return d;
    return ad_primorial_exp(d);
}
