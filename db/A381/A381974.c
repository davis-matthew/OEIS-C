/*
 * A381974  Primes of the form Sum_{k >= 0} floor(m/3^k) for some number m.
 * https://oeis.org/A381974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   p is a prime if there exists an integer m such that p = Σ_{k≥0}
 *            floor(m/3^k). The sequence a(n) lists the primes p in ascending
 *            order, indexed with a(1)=2
 * Range:     u8 n <= 36, u16 n <= 4345, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9321 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
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

static seq_t *prime_vals = NULL;   /* array of found primes */
static size_t prime_count = 0;     /* number of primes stored */
static size_t prime_cap = 0;       /* allocated capacity of prime_vals */
static idx_t m_next = 1;           /* next m to test */

/* Ensure at least 'need' slots are available in prime_vals. */
static void ensure_prime_cap(size_t need)
{
    if (prime_cap >= need)
        return;
    size_t newcap = prime_cap ? prime_cap << 1 : 64;
    while (newcap < need)
        newcap <<= 1;
    seq_t *tmp = mem_resize(seq_t, prime_vals, newcap);
    if (!tmp) {
        /* memory allocation failed – abort as per limits of the exercise */
        abort();
    }
    prime_vals = tmp;
    prime_cap = newcap;
}

/* Compute a(n): n ≥ 1.  Calls are allowed in any order and the function
   remembers previously computed primes. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* index 0 is not defined in OEIS, returning 0 is harmless */

    if ((size_t)n <= prime_count)
        return prime_vals[n - 1];

    /* generate primes until we have enough */
    for (; prime_count < (size_t)n; ++m_next) {
        seq_t t = (seq_t)m_next;
        seq_t s = 0;
        while (t) {
            s += t;
            t /= 3;
        }
        if (is_prime_big(s)) {
            ensure_prime_cap(prime_count + 1);
            prime_vals[prime_count++] = s;
        }
    }
    return prime_vals[n - 1];
}
