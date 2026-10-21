/*
 * A292005  Composite numbers k such that psi(k - phi(k)) = phi(psi(k) - k).
 * https://oeis.org/A292005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth composite k such that psi(k - φ(k)) == φ(ψ(k) -
 *            k). The function searches positive integers until it has found n
 *            qualifying composites, memoising results for repeated calls
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   grow_seq, is_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"
#include "../utils/sieve_primes.h"

/* Compute Euler's totient φ(m).  m is assumed to fit in idx_t. */
static seq_t phi_of(idx_t m, idx_t *primes, size_t prime_cnt)
{
    seq_t result = (seq_t)m;
    idx_t temp   = m;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        idx_t p = primes[i];
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1) result -= result / temp;

    return result;
}

/* Compute Dedekind ψ(m).  ψ(m) = m ∏ (1 + 1/p) over distinct primes p|m. */
static seq_t psi_of(idx_t m, idx_t *primes, size_t prime_cnt)
{
    seq_t result = (seq_t)m;
    idx_t temp   = m;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        idx_t p = primes[i];
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result = result / p * (p + 1);
        }
    }
    if (temp > 1) result = result / temp * (temp + 1);

    return result;
}

seq_t a(idx_t n)
{
    /* Static state shared among calls */
    static idx_t *prime_list      = NULL;
    static size_t prime_count     = 0;

    static seq_t *seq      = NULL;
    static size_t seq_len  = 0;
    static size_t seq_cap  = 0;

    static idx_t next_checked = 1;
    static bool  primes_built = false;

    /* Build a modest prime list once; increase LIMIT if larger terms are needed. */
    if (!primes_built) {
        const idx_t LIMIT = 2000000; /* Should cover n up to at least 10000 */
        sieve_primes(LIMIT, &prime_list, &prime_count);
        primes_built = true;
    }

    /* Extend the sequence until we have at least n terms */
    while (seq_len < n) {
        ++next_checked;

        /* Skip primes – the sequence only contains composites */
        if (is_prime(next_checked)) continue;

        /* For composite k = next_checked compute φ(k) and ψ(k) */
        seq_t phi_k = phi_of(next_checked, prime_list, prime_count);
        seq_t psi_k = psi_of(next_checked, prime_list, prime_count);

        seq_t k_minus_phi = (seq_t)next_checked - phi_k;
        seq_t psi_minus_k = psi_k - (seq_t)next_checked;

        /* ψ(k – φ(k)) and φ(ψ(k) – k) */
        seq_t lhs = psi_of((idx_t)k_minus_phi, prime_list, prime_count);
        seq_t rhs = phi_of((idx_t)psi_minus_k, prime_list, prime_count);

        if (lhs == rhs) {
            /* Store the new term */
            if (seq_len + 1 > seq_cap)
                grow_seq(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = (seq_t)next_checked;
        }
    }

    return seq[n - 1];
}
