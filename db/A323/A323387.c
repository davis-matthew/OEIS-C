/*
 * A323387  Primes whose digits are distinct square digits, i.e., consisting
 *          of only digits 0, 1, 4, 9.
 * https://oeis.org/A323387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: full, pass, 14 terms through n = 14 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating all permutations of the digits 0, 1, 4, 9
   with distinct digits (no leading zero), testing each for primality. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
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

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

static const int DIGITS[4] = {0, 1, 4, 9};
static seq_t primes[64];
static size_t prime_count = 0;
static bool initialized = false;

/* Recursive generation of distinct‑digit numbers. */
static void generate(seq_t current, int depth, int used_mask) {
    if (depth >= 2) {
        if (is_prime_big(current)) {
            primes[prime_count++] = current;
        }
    }
    if (depth == 4) return;
    for (int i = 0; i < 4; ++i) {
        if (used_mask & (1 << i)) continue;        /* already used */
        int d = DIGITS[i];
        if (depth == 0 && d == 0) continue;        /* leading zero disallowed */
        generate(current * 10 + d, depth + 1, used_mask | (1 << i));
    }
}

/* One‑time initialization of the sorted prime list. */
static void init(void) {
    if (initialized) return;
    generate((seq_t)0, 0, 0);
    mem_qsort(primes, prime_count, sizeof(seq_t), cmp_seq_t);
    initialized = true;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n) {
    init();
    if (n == 0) return (seq_t)0;                     /* undefined for n < 1 */
    if ((idx_t)prime_count >= n) return primes[(size_t)(n - 1)];
    return (seq_t)0;                                 /* out of range → 0   */
}
