/*
 * A244748  Numbers k such that (product of digits of k)^2 + 1 is prime.
 * https://oeis.org/A244748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 768, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9489 (2026-11-03)
 * Helpers:   digit_product, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A244748: Numbers k such that (product of digits of k)^2 + 1 is prime. */

/* The function uses a simple incremental search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>
#include <string.h>
#include "../utils/digit_product.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

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

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Static storage for the term list and the generation state. */
    static seq_t *terms = NULL;      /* array of found terms */
    static size_t capacity = 0;      /* allocated capacity of terms */
    static size_t size = 0;          /* number of terms stored */
    static seq_t next_k = 1;         /* next integer to test */

    /* Ensure we have at least n terms precomputed. */
    size_t target = (size_t)n;
    while (size < target) {
        seq_t k = next_k;
        seq_t prod = digit_product(k);
        seq_t val = seq_mul(prod, prod);  /* (product of digits)^2 */
        val += 1;                         /* + 1 */

        if (is_prime_big(val)) {
            /* Grow the array if necessary. */
            if (size == capacity) {
                size_t new_cap = (capacity == 0) ? 128 : capacity * 2;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) abort();  /* allocation failed */
                terms = tmp;
                capacity = new_cap;
            }
            terms[size++] = k;
        }
        next_k++;
    }

    return terms[n - 1];
}
