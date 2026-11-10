/*
 * A347958  Inverse Möbius transform of A345000.
 * https://oeis.org/A347958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n} A345000(d), where A345000(d) = gcd(A003415(d),
 *            A003415(A276086(d)))
 * Range:     u8 n <= 326, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 9845 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t A345000_deriv_uint(uint64_t n)
{
    if (n <= 1) return 0;
    seq_t res = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) { ++e; tmp /= p; }
            res += (seq_t)e * ((seq_t)n / (seq_t)p);
        }
    }
    if (tmp > 1) res += (seq_t)(n / tmp);
    return res;
}

static seq_t A345000_powmod_seq(seq_t base, idx_t exp, seq_t mod)
{
    if (mod == 0) return 0;
    seq_t res = 1 % mod;
    while (exp) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

#define A345000_MAX_PRIMES 64
static uint64_t A345000_rep_primes[A345000_MAX_PRIMES];
static size_t A345000_rep_prime_cnt = 0;

static void A345000_add_next_prime(void)
{
    uint64_t candidate = A345000_rep_prime_cnt == 0 ? 2 : A345000_rep_primes[A345000_rep_prime_cnt-1] + 1;
    while (1) {
        bool prime = true;
        for (size_t j = 0; j < A345000_rep_prime_cnt; ++j) {
            if (candidate % A345000_rep_primes[j] == 0) { prime = false; break; }
        }
        if (prime) break;
        ++candidate;
    }
    if (A345000_rep_prime_cnt < A345000_MAX_PRIMES) A345000_rep_primes[A345000_rep_prime_cnt++] = candidate;
}

static void A345000_ensure_rep_primes(size_t needed)
{
    while (A345000_rep_prime_cnt < needed && A345000_rep_prime_cnt < A345000_MAX_PRIMES) A345000_add_next_prime();
}

static seq_t A345000_a(idx_t n)
{
    uint64_t nn = n;

    if (nn == 0) return 0;
    if (nn == 1) return 1;

    seq_t Dn = A345000_deriv_uint(nn);

    A345000_ensure_rep_primes(1);
    seq_t primorials[A345000_MAX_PRIMES+1];
    size_t k = 0;
    unsigned __int128 prod = 1;
    primorials[0] = 1;
    while (1) {
        A345000_ensure_rep_primes(k+1);
        uint64_t prime = A345000_rep_primes[k];
        unsigned __int128 next = prod * prime;
        if (next > nn) break;
        prod = next;
        primorials[++k] = (seq_t)prod;
    }
    size_t len = k + 1;

    uint64_t m = nn;
    unsigned digits[A345000_MAX_PRIMES+1] = {0};
    for (int i = (int)k; i >= 0; --i) {
        unsigned d = (unsigned)(m / (uint64_t)primorials[i]);
        digits[i] = d;
        m -= (uint64_t)d * (uint64_t)primorials[i];
    }

    seq_t pow_i[A345000_MAX_PRIMES+1];
    for (size_t i = 0; i < len; ++i) {
        pow_i[i] = A345000_powmod_seq((seq_t)A345000_rep_primes[i], digits[i], Dn);
    }

    seq_t prefix[A345000_MAX_PRIMES+1], suffix[A345000_MAX_PRIMES+1];
    if (len > 0) {
        prefix[0] = pow_i[0] % Dn;
        for (size_t i = 1; i < len; ++i)
            prefix[i] = (prefix[i-1] * pow_i[i]) % Dn;

        suffix[len-1] = pow_i[len-1] % Dn;
        for (int i = (int)len-2; i >= 0; --i)
            suffix[i] = (suffix[i+1] * pow_i[i]) % Dn;
    }

    seq_t D2Mod = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned e = digits[i];
        if (e == 0) continue;

        seq_t prod_except;
        if (i == 0)            prod_except = suffix[1];
        else if (i == len-1)   prod_except = prefix[len-2];
        else                   prod_except = (prefix[i-1] * suffix[i+1]) % Dn;

        seq_t p_ex1 = A345000_powmod_seq((seq_t)A345000_rep_primes[i], e-1, Dn);

        seq_t term = ((seq_t)e * ((prod_except * p_ex1) % Dn)) % Dn;
        D2Mod += term;
        D2Mod %= Dn;
    }

    return gcd(Dn, D2Mod);
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    if (n == 0) return 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            total += A345000_a(d);
            idx_t q = n / d;
            if (q != d) total += A345000_a(q);
        }
    }
    return total;
}
