/*
 * A243072  Second order Bulgarian solitaire operation on partition list
 *          A112798: a(1) = 1, a(n) = A000040(A001222(n)) *
 *          A242424(A064989(n)).
 * https://oeis.org/A243072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = p_{Ω(n)} * p_t * ∏_{p^e‖n, p≥5} p_{index(p)-2}^e *
 *            where * Ω(n) = total number of prime factors of n (counted with
 *            multiplicity) * t = total number of prime factors of n that are
 *            ≥ 3 * p_k = k‑th prime (p_1 = 2, p_2 = 3, …) * Special cases:
 *            a(1) = 1. If Ω(n)=0 or t=0 the corresponding fa...
 * Range:     u8 n <= 72, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   build_primes_upto, factor_u64, get_prime, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/build_primes_upto.h"

/* ---------- dynamic prime list ----------
 * primes_global holds the list of all primes up to prime_limit_global.
 * prime_index_of(p) guarantees that the list covers p using
 * build_primes_upto when necessary and returns the 1‑based index.
 */
static uint64_t *primes_global = NULL;
static size_t   prime_count_global = 0;
static idx_t    prime_limit_global = 0;

/* allocate enough primes up to 'limit' (inclusive) */
static void ensure_prime_limit(idx_t limit)
{
    if (limit <= prime_limit_global)
        return;
    /* build new list and replace the old one */
    uint64_t *new_primes = NULL;
    size_t   new_count   = 0;
    build_primes_upto(limit, &new_primes, &new_count);
    free(primes_global);
    primes_global = new_primes;
    prime_count_global = new_count;
    prime_limit_global = limit;
}

/* binary search in the prime list (1‑based indices returned) */
static idx_t prime_index_of(uint64_t p)
{
    ensure_prime_limit(p);
    idx_t lo = 0, hi = (idx_t)prime_count_global - 1;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (primes_global[mid] == p)
            return mid + 1;               /* 1‑based index            */
        if (primes_global[mid] < p)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return 0;  /* should never happen for valid primes */
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 1)               /* a(1) is defined to be 1 */
        return 1;

    /* factorise n */
    uint64_t fp[16];          /* prime factors */
    uint64_t fe[16];          /* exponents   */
    size_t   cnt = 0;
    factor_u64((uint64_t)n, fp, fe, &cnt);

    idx_t omega = 0;          /* Ω(n) = total multiplicity */
    idx_t t     = 0;          /* count of factors ≥ 3 */
    seq_t prod  = 1;          /* product of p_{idx(p)-2}^e for p≥5 */

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = fp[i];
        idx_t    e = (idx_t)fe[i];
        omega += e;

        idx_t idx = prime_index_of(p);
        if (p >= 3)   /* contributes to t (all primes except 2) */
            t += e;

        if (p >= 5)   /* contributes to the product */
        {
            idx_t idx_minus_2 = idx - 2;   /* ≥ 1 because p≥5 ⇒ idx≥3 */
            seq_t prime_val   = (seq_t)get_prime(idx_minus_2);
            seq_t pow_val     = seq_pow(prime_val, e);
            prod = seq_mul(prod, pow_val);
        }
    }

    seq_t result = prod;

    /* multiply by p_{Ω(n)} (if Ω(n) > 0) */
    if (omega > 0)
    {
        seq_t prime_omega = (seq_t)get_prime(omega);
        result = seq_mul(result, prime_omega);
    }

    /* multiply by p_t (if t > 0) */
    if (t > 0)
    {
        seq_t prime_t = (seq_t)get_prime(t);
        result = seq_mul(result, prime_t);
    }

    return result;
}
