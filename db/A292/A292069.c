/*
 * A292069  Primes of the form (24^k - 1) / 23.
 * https://oeis.org/A292069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (24^k - 1)/23 where k is the n‑th index for which that
 *            value is prime. The values are produced by iterating k and
 *            testing primality of the recurrence A_k = 24*A_(k-1) + 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* static state */
    static idx_t       current_k      = 1;           /* current exponent k   */
    static seq_t       current_value  = 1;            /* A_k = (24^k-1)/23     */
    static seq_t *     primes         = NULL;         /* array of found primes */
    static size_t      primes_cap     = 0;            /* capacity of array     */
    static size_t      prime_count    = 0;            /* number of primes     */

    /* grow primes array if necessary */
    if (prime_count >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 8;
        seq_t *tmp = seq_realloc(primes, primes_cap, new_cap);
        if (!tmp) return 0;          /* allocation failure */
        primes = tmp;
        primes_cap = new_cap;
    }

    /* generate primes until we have at least n */
    while (prime_count < (size_t)n) {
        ++current_k;
        /* next A_k = 24*A_(k-1) + 1 */
        current_value = current_value * 24 + 1;

        /* skip composite k (they cannot yield prime A_k) */
        if (!is_prime(current_k))
            continue;

        if (is_prime_big(current_value)) {
            primes[prime_count++] = current_value;
        }
    }

    return primes[n-1];
}
