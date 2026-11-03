/*
 * A386181  Primes having only {3, 5, 7, 8} as digits.
 * https://oeis.org/A386181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth prime whose decimal digits belong to {3,5,7,8}
 * Range:     u8 n <= 7, u16 n <= 113, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9841 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
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

static seq_t *primes = NULL;             /* array of found primes */
static size_t primes_len = 0;            /* number of primes stored */
static size_t primes_cap = 0;            /* allocated capacity */

static const unsigned int allowed_digits[4] = {3, 5, 7, 8};

static int current_len = 0;              /* number of digits processed */

/* Recursive generation of all numbers of a given length comprising only
   the allowed digits.  For each complete number it checks primality
   and, if prime, adds it to the global array. */
static void generate_rec(seq_t cur, int depth, int target)
{
    if (depth == target) {
        if (is_prime_big(cur)) {
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                if (!tmp) return;          /* allocation failed – abort */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = cur;
        }
        return;
    }
    for (int i = 0; i < 4; ++i) {
        generate_rec(cur * 10 + allowed_digits[i], depth + 1, target);
    }
}

/* Ensure that the global prime array contains at least `needed` primes.
   It generates primes length by length until the required amount is
   reached. */
static void ensure_enough(size_t needed)
{
    while (primes_len < needed) {
        ++current_len;
        generate_rec(0, 0, current_len);
    }
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety – sequence is 1‑based */
    ensure_enough((size_t)n);
    return primes[n - 1];
}
