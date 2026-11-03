/*
 * A257937  Primes p such that one digit appears exactly six times together
 *          with a single different digit.
 * https://oeis.org/A257937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: full, pass, 35 terms through n = 35 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes with exactly six identical digits and one distinct digit
   (OEIS A257937).  The set of qualifying numbers is fixed for 7‑digit
   integers, so we generate all candidates, test for primality, sort
   the primes once, and then answer queries from this table. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Static cache of primes once produced */
static seq_t primes[250];   /* 248 possible numbers, 250 is safe */
static size_t prime_count = 0;

/* Random access to the first primality table and sorting */
static void init_primes(void)
{
    if (prime_count != 0)                /* already initialised */
        return;

    const int repeated_digits[4] = {1, 3, 7, 9};
    const int total_positions   = 7;     /* 7‑digit numbers */

    for (int r = 0; r < 4; ++r) {
        int d = repeated_digits[r];      /* repeated digit */
        for (int e = 0; e <= 9; ++e) {   /* distinct digit */
            if (e == d) continue;        /* must differ from d */
            for (int pos = 0; pos < total_positions; ++pos) {
                /* Skip leading zero */
                if (pos == 0 && e == 0)
                    continue;

                /* Build the numeric value */
                uint64_t val = 0;
                int digits[7];
                for (int i = 0; i < total_positions; ++i)
                    digits[i] = d;
                digits[pos] = e;
                for (int i = 0; i < total_positions; ++i)
                    val = val * 10 + digits[i];

                seq_t v = (seq_t)val;      /* cast is safe for all builds */

                if (is_prime_big(v))
                    primes[prime_count++] = v;
            }
        }
    }

    /* Simple insertion sort – adequate for ≤250 items and works
       for both plain integer types and arbitrary‑precision classes. */
    for (size_t i = 0; i + 1 < prime_count; ++i) {
        for (size_t j = i + 1; j < prime_count; ++j) {
            if (primes[j] < primes[i]) {
                seq_t tmp = primes[i];
                primes[i] = primes[j];
                primes[j] = tmp;
            }
        }
    }
}

/* Public implementation of the OEIS sequence A257937 */
seq_t a(idx_t n)
{
    init_primes();               /* ensure table built */

    if (n == 0 || n > prime_count)
        return 0;                /* undefined for out‑of‑range indices */

    return primes[n - 1];
}
