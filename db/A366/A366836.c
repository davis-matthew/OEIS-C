/*
 * A366836  a(1) = 1 and a(n) = prime(a(n-1)+n) mod (a(n-1)+n).
 * https://oeis.org/A366836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(a(n‑1)+n) mod (a(n‑1)+n) a(1) = 1
 * Range:     u8 n <= 27, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   Dynamic prime generator
   Stores primes in a growing array and extends it lazily as more
   primes are required.  The primality test is simple trial division
   using the already known primes; this is fast enough for n ≤ 10000.
   ------------------------------------------------------------------ */

static uint32_t *prime_list   = NULL;   /* 1‑indexed array of primes */
static size_t   prime_count  = 0;       /* number of primes stored   */
static uint32_t last_checked = 1;       /* last examined integer    */

static void
_ensure_primes(size_t needed)
{
    if (prime_count >= needed) return;

    size_t new_cap = prime_count ? prime_count << 1 : 8;
    if (new_cap < needed) new_cap = needed;

    /* realloc may return NULL; the assignment is safe because we
       never use the array until after the call.  The cast is required
       for C++ compatibility. */
    prime_list = (uint32_t *)realloc(prime_list, new_cap * sizeof *prime_list);

    for (uint32_t cand = last_checked + 1; ; ++cand)
    {
        bool isprime = true;
        for (size_t i = 0; i < prime_count; ++i)
        {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * (uint64_t)p > cand) break;
            if (cand % p == 0) { isprime = false; break; }
        }
        if (isprime)
        {
            prime_list[prime_count++] = cand;
            last_checked = cand;
            if (prime_count >= needed) return;
        }
    }
}

/* ------------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below the offset – unused */

    seq_t prev = 1;                 /* a(1) */
    if (n == 1) return prev;

    for (idx_t i = 2; i <= n; ++i)
    {
        idx_t m_idx = (idx_t)prev + i;          /* a(n‑1)+n          */
        _ensure_primes((size_t)m_idx);         /* need m‑th prime   */
        seq_t prime_val = (seq_t)prime_list[(size_t)m_idx - 1];
        prev = prime_val % (seq_t)m_idx;       /* a(n) = prime(m) % m */
    }
    return prev;
}
