/*
 * A249606  Primes of the form 2k^2 + k + 2.
 * https://oeis.org/A249606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form 2k^2 + k + 2, n >= 1
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

static seq_t *primes_of_form = NULL;   /* cache of found primes */
static size_t primes_cap     = 0;      /* allocated capacity */
static size_t primes_len     = 0;      /* number of primes stored */
static idx_t   next_k        = 0;      /* next k to test */

static void ensure_capacity(void)
{
    if (primes_len >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        seq_t *new_arr = mem_new(seq_t, new_cap);
        if (!new_arr) exit(1); /* allocation failure */
        if (primes_of_form) {
            mem_copy(new_arr, primes_of_form, primes_len * sizeof(seq_t));
            mem_free(primes_of_form);
        }
        primes_of_form = new_arr;
        primes_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n=0, but return 0 */

    /* compute primes until we have at least n primes */
    while (primes_len < n) {
        idx_t       k       = next_k++;
        seq_t       k_seq   = (seq_t)k;
        seq_t       p       = 2 * k_seq * k_seq + k_seq + 2; /* 2k^2 + k + 2 */

        if (is_prime_big(p)) {
            ensure_capacity();
            primes_of_form[primes_len++] = p;
        }
    }

    return primes_of_form[n - 1];
}
