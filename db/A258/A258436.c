/*
 * A258436  Primes p of form x^2 - phi(x) such that (p-1)/tau(p-1) is also
 *          prime.
 * https://oeis.org/A258436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime p of the form p = x^2 − φ(x) * such that
 *            (p − 1)/τ(p − 1) is also prime, where τ is the number * of
 *            divisors. (OEIS A258436)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_divisors, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"
#include "../utils/totient_seq.h"
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

static seq_t *cache   = NULL;   /* dynamic array of found primes */
static size_t cache_cap = 0;    /* current capacity of the array */
static idx_t   cache_size = 0;  /* number of primes stored */
static idx_t   next_x = 2;      /* next x to test */

/* Append a prime to the cache, growing the array if needed. */
static void append_prime(seq_t p)
{
    /* Ensure there is space for one more element. */
    grow_seq(&cache, &cache_cap, 1);
    cache[cache_size++] = p;
}

/* Generate primes until at least n terms are stored. */
static void generate_until(idx_t n)
{
    while (cache_size < n)
    {
        seq_t x        = (seq_t)next_x;
        seq_t phi_x    = totient_seq(x);
        seq_t p        = x * x - phi_x;            /* candidate prime */

        /* p must be > 0 and fit into 64‑bit for the divisor test */
        if (p > (seq_t)0 && p <= (seq_t)UINT64_MAX)
        {
            if (is_prime_big(p))
            {
                uint64_t p_minus1 = (uint64_t)(p - 1);
                uint64_t tau     = num_divisors(p_minus1);

                /* tau must divide (p-1) exactly */
                if (tau != 0 && p_minus1 % tau == 0)
                {
                    uint64_t ratio = p_minus1 / tau;
                    if (is_prime_big((seq_t)ratio))
                    {
                        append_prime(p);
                    }
                }
            }
        }

        ++next_x;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; undefined for 0 */
    generate_until(n);
    return cache[n - 1];
}
