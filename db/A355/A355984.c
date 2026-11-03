/*
 * A355984  Primes whose reversal is a multiple of 8.
 * https://oeis.org/A355984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th prime whose decimal reversal is a multiple of 8
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by linear search with memoisation.          */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/reverse_digits.h"
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

seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *terms       = NULL;
    static size_t terms_len   = 0;
    static size_t terms_cap   = 0;

    /* Next integer to test for primality (64‑bit is sufficient for the
       intended limits; cast to seq_t for the primality test). */
    static uint64_t next_candidate = 2;

    if (n < (idx_t)1) return (seq_t)0;

    /* Grow the cache until it contains at least n terms. */
    while (terms_len < (size_t)n) {
        /* Find the next prime ≥ next_candidate. */
        while (!is_prime_big((seq_t)next_candidate)) {
            ++next_candidate;
        }

        /* Apply the defining condition. */
        seq_t rev = reverse_digits((seq_t)next_candidate);
        if ((rev % (seq_t)8) == (seq_t)0) {
            /* Allocate space for a new entry if needed. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) abort();           /* Out of memory. */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)next_candidate;
        }

        /* Move to the next integer for the next iteration. */
        ++next_candidate;
    }

    return terms[n - 1];   /* a(1) is stored at index 0. */
}
