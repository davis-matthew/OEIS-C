/*
 * A394048  The product of the smallest primes in every congruence class
 *          modulo n which are relatively prime to n.
 * https://oeis.org/A394048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): product of the smallest primes in each residue class
   modulo n that are coprime to n (OEIS A394048). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----------  Greatest Common Divisor for idx_t (uint64_t)  ---------- */
static inline idx_t my_gcd(idx_t a, idx_t b)
{
    while (b)
    {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* ----------  Euler's totient φ(n) for small n  ---------- */
static idx_t phi_uint64(idx_t n)
{
    idx_t result = n;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            result = result / p * (p - 1);
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) result = result / m * (m - 1);
    return result;
}

/* ----------  Global prime table (dynamic)  ---------- */
static uint64_t *global_primes = NULL;
static size_t   global_prime_count = 0;
static idx_t    global_prime_limit = 0;

/* Ensure that global_primes contains all primes up to 'limit',
   rebuilding the table if necessary.  Each rebuild reallocates
   the array and re‑sieves the whole range. */
static void ensure_primes_at_least(idx_t limit)
{
    if (limit <= global_prime_limit) return;

    /* Free old table if it exists. */
    free(global_primes);

    /* Simple sieve up to "limit". */
    bool *is_comp = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_comp) exit(1);

    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
    {
        if (!is_comp[i])
        {
            ++cnt;
            if (i * i <= (size_t)limit)
            {
                for (size_t j = i * i; j <= (size_t)limit; j += i)
                    is_comp[j] = true;
            }
        }
    }

    global_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!global_primes) exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
    {
        if (!is_comp[i])
            global_primes[idx++] = (uint64_t)i;
    }

    global_prime_count = cnt;
    global_prime_limit = limit;
    free(is_comp);
}

/* ----------  Main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;                /* offset 1 */

    /* Ensure we have at least a few primes. */
    ensure_primes_at_least(20);

    /* seen[i] == true  <=>  residue i (mod n) already has its smallest prime. */
    bool *seen = (bool *)calloc((size_t)n, sizeof(bool));
    if (!seen) exit(1);

    idx_t remaining = phi_uint64(n);
    seq_t product = (seq_t)1;

    size_t prime_idx = 0;
    while (remaining > 0)
    {
        if (prime_idx >= global_prime_count)
        {
            idx_t new_limit = global_prime_limit * 2 + 1000;
            if (new_limit < global_prime_limit + 1000) new_limit = global_prime_limit + 1000;
            ensure_primes_at_least(new_limit);
        }

        uint64_t p = global_primes[prime_idx++];
        if (my_gcd((idx_t)p, n) != 1) continue;

        idx_t r = (idx_t)(p % n);
        if (!seen[r])
        {
            seen[r] = true;
            product *= (seq_t)p;
            --remaining;
        }
    }

    free(seen);
    return product;
}
