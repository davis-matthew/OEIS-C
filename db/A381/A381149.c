/*
 * A381149  a(1) = 2, a(2) = 3; thereafter, a(n) = a(n-1) + sum of prior prime
 *          terms.
 * https://oeis.org/A381149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 32, u64 n <= 212, u128 n <= 329,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A381149:
   a(1)=2, a(2)=3.
   For n>2: a(n)=a(n-1)+S(n-1)
   where S(n-1)=Σ{a(k) | 1≤k≤n-1 and a(k) is prime}.
   The implementation memoizes terms and the running sum of primes.
   A custom deterministic Miller–Rabin test is used for 64‑bit
   sequence values; for larger (arbitrary‑precision) builds the
   supplied is_prime_big() is used. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *seq_data = NULL;      /* computed terms */
static size_t seq_cap = 0;          /* capacity of seq_data   */
static size_t seq_len = 0;          /* number of terms stored */
static seq_t sum_primes = (seq_t)0; /* sum of all prime terms so far */

/* -------------------  small types prime test ------------------- */
#if defined(__UINT128_TYPE__) || defined(__SIZEOF_INT128__)
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    /* deterministic bases for 64-bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t bi = 0; bi < sizeof(bases)/sizeof(bases[0]); ++bi) {
        uint64_t a = bases[bi] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a;
        uint64_t e = d;
        while (e) {
            if (e & 1) x = (x * p) % n;
            p = (p * p) % n;
            e >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}
#endif

/* generic prime test for seq_t */
static bool is_prime_seq(seq_t n)
{
#if defined(__UINT128_TYPE__) || defined(__SIZEOF_INT128__)
    /* use fast test when seq_t is 64-bit */
    if (sizeof(seq_t) == sizeof(uint64_t))
        return is_prime_uint64((uint64_t)n);
#endif
    /* fallback to utility for arbitrary precision */
    return is_prime_big(n);
}

/* -------------------  dynamic array helpers ------------------- */
static void ensure_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? (seq_cap << 1) : 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_data = (seq_t *)realloc(seq_data, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* -----------------------  main routine ----------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined, return 0 */

    /* compute terms up to n if necessary */
    while (seq_len < (size_t)n) {
        seq_t next_val;
        if (seq_len == 0) {             /* a(1) = 2 */
            next_val = (seq_t)2;
            sum_primes += next_val;
        } else if (seq_len == 1) {      /* a(2) = 3 */
            next_val = (seq_t)3;
            sum_primes += next_val;
        } else {
            /* sum of primes among a(1)..a(seq_len) is stored in sum_primes */
            next_val = seq_data[seq_len - 1] + sum_primes;
            if (is_prime_seq(next_val))
                sum_primes += next_val;
        }

        ensure_cap(seq_len + 1);
        seq_data[seq_len] = next_val;
        ++seq_len;
    }

    return seq_data[(size_t)n - 1];
}
