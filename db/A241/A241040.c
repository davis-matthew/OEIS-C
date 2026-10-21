/*
 * A241040  Differences between primes having primitive root 2.
 * https://oeis.org/A241040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241040: Differences between consecutive primes for which 2 is a
   primitive root.  a(n) = p_{n+1} – p_n where p_k is the k‑th prime with
   2 a primitive root. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/powmod64.h"

/* Return true iff 2 is a primitive root modulo the prime p. */
static bool has_primitive_root_2(uint64_t p)
{
    if (p <= 2)
        return false;                /* 2 is not a primitive root mod 2 */

    uint64_t phi = p - 1;            /* φ(p) = p-1 for prime p */
    uint64_t m   = phi;
    uint64_t q   = 2;

    /* Factor φ(p) and test the condition for each prime divisor. */
    while (q * q <= m) {
        if (m % q == 0) {
            if (powmod64(2ULL, phi / q, p) == 1ULL)
                return false;
            while (m % q == 0)
                m /= q;
        }
        q += (q == 2) ? 1 : 2;       /* try only odd q after 2 */
    }
    if (m > 1) {                     /* remaining prime factor > sqrt(φ) */
        if (powmod64(2ULL, phi / m, p) == 1ULL)
            return false;
    }
    return true;
}

/* Dynamic array of primes that have primitive root 2. */
static uint64_t      *primes_arr  = NULL;
static size_t        primes_cap  = 0;
static size_t        primes_cnt  = 0;

/* Ensure capacity for at least new_cap elements. */
static void ensure_primes_capacity(size_t new_cap)
{
    if (primes_cap >= new_cap)
        return;

    size_t cap = (primes_cap == 0) ? 16 : primes_cap * 2;
    if (cap < new_cap)
        cap = new_cap;

    uint64_t *tmp = (uint64_t *)realloc(primes_arr, cap * sizeof(uint64_t));
    /* For this exercise we assume realloc succeeds. */
    primes_arr = tmp;
    primes_cap = cap;
}

/* Append prime p to the dynamic array. */
static void add_prime(uint64_t p)
{
    ensure_primes_capacity(primes_cnt + 1);
    primes_arr[primes_cnt++] = p;
}

/* Generate enough primes with primitive root 2 to cover at least need_cnt
   elements (1‑based index). */
static void ensure_primes(size_t need_cnt)
{
    if (primes_cnt >= need_cnt)
        return;

    uint64_t candidate = 3; /* 2 is not in the list */
    if (primes_cnt > 0)
        candidate = primes_arr[primes_cnt - 1] + 2; /* jump to next odd */

    while (primes_cnt < need_cnt) {
        if (is_prime((idx_t)candidate) && has_primitive_root_2(candidate))
            add_prime(candidate);
        candidate += 2; /* test only odd numbers */
    }
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but safe fallback */

    /* We need the first n+1 such primes to compute a(n). */
    size_t need = (size_t)n + 1;
    ensure_primes(need);

    /* Indexing: primes_arr[0] = first such prime. */
    size_t idx = (size_t)n; /* difference between primes_arr[idx] and primes_arr[idx-1] */
    seq_t diff = (seq_t)(primes_arr[idx] - primes_arr[idx - 1]);

    return diff;
}
