/*
 * A377432  Number of perfect-powers x in the range prime(n) < x < prime(n+1).
 * https://oeis.org/A377432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: count perfect‑powers in the open interval
   between prime(n) and prime(n+1) using incremental prime generation
   and brute‑force perfect‑power testing. */

#include <stddef.h>
#include <stdbool.h>

/* Test whether x is a perfect power (x = a^k, a>1, k>1) using only
   integer arithmetic.  This routine works for any seq_t that supports
   the usual operators. */
static bool is_perfect_power_seq(seq_t x)
{
    if (x < 4) return false;
    for (seq_t base = 2; base * base <= x; ++base) {
        seq_t power = 1;
        while (power < x) {
            power = power * base;
            if (power == x) return true;
        }
    }
    return false;
}

/* A static cache for the first MAX_PRIME_CACHE primes.
   For the purposes of this sequence MAX_PRIME_CACHE > 10000 is ample. */
#define MAX_PRIME_CACHE 200000
static idx_t prime_cache[MAX_PRIME_CACHE];
static size_t prime_used = 0;

/* Generate primes until we have at least 'needed' primes in prime_cache. */
static void ensure_primes_upto(idx_t needed)
{
    if (prime_used >= needed || needed > MAX_PRIME_CACHE) return;
    if (prime_used == 0) {
        prime_cache[0] = 2;
        prime_used = 1;
    }
    idx_t candidate = prime_cache[prime_used - 1] + 1;
    while (prime_used < needed) {
        bool is_p = true;
        for (size_t i = 0; i < prime_used; ++i) {
            idx_t p = prime_cache[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_p = false; break; }
        }
        if (is_p) {
            prime_cache[prime_used++] = candidate;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    /* need prime(n) and prime(n+1) → generate up to n+1 primes */
    ensure_primes_upto(n + 1);
    idx_t p_prev = prime_cache[n - 1];
    idx_t p_next = prime_cache[n];
    seq_t count = 0;
    for (idx_t x = p_prev + 1; x < p_next; ++x) {
        if (is_perfect_power_seq((seq_t)x))
            count = count + 1;
    }
    return count;
}
