/*
 * A386020  Primes having only {0, 1, 2, 7} as digits.
 * https://oeis.org/A386020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 113, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9412 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search over integers whose decimal digits belong to {0,1,2,7},
   test each for primality, and return the nth such prime.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static const seq_t allowed_digits[4] = {0u, 1u, 2u, 7u};

/* Recursive generator: visit all numbers of a fixed length 'len' in
   numeric order (digits are tried in increasing order).  Whenever a
   prime is found it is appended to global array 'primes'.  */
static void dfs_generate(int pos, int len, seq_t current,
                         seq_t **primes, size_t *cap, idx_t *cnt)
{
    if (pos > len) {                     /* number complete */
        if (is_prime_big(current)) {
            (*cnt)++;
            grow_seq(primes, cap, (size_t)*cnt);
            (*primes)[*cnt - 1] = current;
        }
        return;
    }

    for (int d = 0; d < 4; ++d) {
        if (pos == 1 && allowed_digits[d] == 0)
            continue;                    /* no leading zero */
        seq_t next = current * 10 + allowed_digits[d];
        dfs_generate(pos + 1, len, next, primes, cap, cnt);
    }
}

/* Public sequence function requested by the build system. */
seq_t a(idx_t n)
{
    static seq_t *primes = NULL;          /* collected primes so far   */
    static size_t primes_cap = 0;
    static idx_t prime_count = 0;         /* number of primes collected */
    static int current_len = 1;           /* current length being generated */

    /* Generate primes until we have at least n of them. */
    while (prime_count < n) {
        dfs_generate(1, current_len, (seq_t)0,
                     &primes, &primes_cap, &prime_count);
        ++current_len;
    }
    return primes[n - 1];
}
