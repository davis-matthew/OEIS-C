/*
 * A345000  a(n) = gcd(A003415(n), A003415(A276086(n))), where A003415(n) is
 *          the arithmetic derivative of n, and A276086(n) gives the prime
 *          product form of primorial base expansion of n.
 * https://oeis.org/A345000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = gcd(A003415(n), A003415(A276086(n)))
 * Range:     u8 n <= 386, u16 n <= 6876, u32 n <= 6876, u64 n <= 6876,
 *            u128 n <= 6876, big any n
 * Helpers:   deriv_uint, gcd, powmod_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the arithmetic derivative of a non‑negative integer n
 * and then returns the required gcd with the derivative of its
 * primorial‑base expansion.
 *
 * All arithmetic is performed using the integer type seq_t which
 * may be 8, 16, 32, 64 bits or an arbitrary‑precision integer class.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/deriv_uint.h"
#include "../utils/powmod_seq.h"

/* ----------------------------------------------------------------
 * Prime handling for primorial base representation.
 * ---------------------------------------------------------------*/
#define MAX_PRIMES 64
static uint64_t rep_primes[MAX_PRIMES];
static size_t rep_prime_cnt = 0;

/* Find next prime after 'candidate' using trial division by known primes. */
static void add_next_prime(void)
{
    uint64_t candidate = rep_prime_cnt == 0 ? 2 : rep_primes[rep_prime_cnt-1] + 1;
    while (1) {
        bool prime = true;
        for (size_t j = 0; j < rep_prime_cnt; ++j) {
            if (candidate % rep_primes[j] == 0) { prime = false; break; }
        }
        if (prime) break;
        ++candidate;
    }
    if (rep_prime_cnt < MAX_PRIMES) rep_primes[rep_prime_cnt++] = candidate;
}

static void ensure_rep_primes(size_t needed)
{
    while (rep_prime_cnt < needed && rep_prime_cnt < MAX_PRIMES) add_next_prime();
}

/* ----------------------------------------------------------------
 * Main sequence function.
 * ---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    uint64_t nn = n;                     /* convert to native integer */

    if (nn == 0) return 0;
    if (nn == 1) return 1;

    /* Derivative of n */
    seq_t Dn = deriv_uint(nn);

    /* Primorial base representation --------------------------------*/
    /* Build required primes and primorials */
    ensure_rep_primes(1);                /* make sure at least prime 2 exists */
    seq_t primorials[MAX_PRIMES+1];
    size_t k = 0;                        /* number of primes used (max index) */
    unsigned __int128 prod = 1;
    primorials[0] = 1;                   /* P0 = 1 */
    while (1) {
        ensure_rep_primes(k+1);
        uint64_t prime = rep_primes[k];
        unsigned __int128 next = prod * prime;
        if (next > nn) break;
        prod = next;
        primorials[++k] = (seq_t)prod;
    }
    size_t len = k + 1;                  /* number of digit positions */

    /* Compute digits of n in primorial base */
    uint64_t m = nn;
    unsigned digits[MAX_PRIMES+1] = {0};
    for (int i = (int)k; i >= 0; --i) {
        unsigned d = (unsigned)(m / (uint64_t)primorials[i]);
        digits[i] = d;
        m -= (uint64_t)d * (uint64_t)primorials[i];
    }

    /* Pow_i modulo Dn (p_i^e_i mod Dn) */
    seq_t pow_i[MAX_PRIMES+1];
    for (size_t i = 0; i < len; ++i) {
        pow_i[i] = powmod_seq((seq_t)rep_primes[i], digits[i], Dn);
    }

    /* Prefix and suffix products of pow_i modulo Dn */
    seq_t prefix[MAX_PRIMES+1], suffix[MAX_PRIMES+1];
    if (len > 0) {
        prefix[0] = pow_i[0] % Dn;
        for (size_t i = 1; i < len; ++i)
            prefix[i] = (prefix[i-1] * pow_i[i]) % Dn;

        suffix[len-1] = pow_i[len-1] % Dn;
        for (int i = (int)len-2; i >= 0; --i)
            suffix[i] = (suffix[i+1] * pow_i[i]) % Dn;
    }

    /* Derivative of A (mod Dn) */
    seq_t D2Mod = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned e = digits[i];
        if (e == 0) continue;

        seq_t prod_except;
        if (i == 0)            prod_except = suffix[1];
        else if (i == len-1)   prod_except = prefix[len-2];
        else                   prod_except = (prefix[i-1] * suffix[i+1]) % Dn;

        seq_t p_ex1 = powmod_seq((seq_t)rep_primes[i], e-1, Dn);   /* p_i^(e-1) mod Dn */

        seq_t term = ((seq_t)e * ((prod_except * p_ex1) % Dn)) % Dn;
        D2Mod += term;
        D2Mod %= Dn;
    }

    /* gcd of Dn and derivative of A */
    return gcd(Dn, D2Mod);
}
