/*
 * A376205  Prime indices of triangular numbers k such that k + 1 and k - 2
 *          are not prime.
 * https://oeis.org/A376205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p such that the triangular number * T(p) =
 *            p·(p+1)/2 satisfies (T(p)+1) and (T(p)-2) are not prime
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
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

/* Return the nth term of OEIS sequence A376205. */
seq_t a(idx_t n)
{
    static seq_t *seq      = NULL;   /* array of found prime indices            */
    static size_t seq_cap = 0;       /* allocated capacity                      */
    static size_t seq_len = 0;       /* number of terms stored (seq_len >= n needed) */
    static idx_t  next_prime_candidate = 2;  /* next integer to test for primality */

    /* Extend the stored sequence if necessary. */
    while (seq_len < n) {
        /* Find the next prime candidate. */
        while (!is_prime(next_prime_candidate))
            ++next_prime_candidate;

        /* Compute the triangular number T(p) = p * (p + 1) / 2. */
        seq_t p = (seq_t)next_prime_candidate;
        seq_t t = p * (p + 1) / 2;

        /* Test the conditions on T(p)+1 and T(p)-2. */
        bool bad = false;
        if (is_prime_big(t + 1))
            bad = true;
        else if (is_prime_big(t - 2))
            bad = true;

        /* If both are composite, store the prime index. */
        if (!bad) {
            /* Ensure capacity of the array. */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, seq, new_cap);
                if (!tmp) return 0;     /* allocation failure (unlikely in test) */
                seq      = tmp;
                seq_cap  = new_cap;
            }
            seq[seq_len++] = p;
        }

        /* Proceed to the next number. */
        ++next_prime_candidate;
    }

    /* Return the nth term (1‑based index). */
    return seq[n - 1];
}
