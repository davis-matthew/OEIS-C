/*
 * A395346  Primes that begin a pair of consecutive primes with the same
 *          number of digits and a prime-valued digit dot product.
 * https://oeis.org/A395346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th prime p such that the next prime q * has the
 *            same number of decimal digits and the digit dot product * p·q =
 *            Σ (digit_i(p) * digit_i(q)) is itself a prime
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem, next_prime, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"
#include "../utils/num_digits0.h"
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
    /* static cache of found terms */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static uint64_t candidate = 2;      /* number to test for primality */

    /* if called with n==0 we return 0 (undefined but harmless) */
    if (n == 0)
        return 0;

    /* Ensure we have at least n terms in the cache */
    while (terms_len < (size_t)n)
    {
        /* Find the next prime candidate ≥ candidate */
        while (!is_prime((idx_t)candidate))
            ++candidate;

        uint64_t p = candidate;
        uint64_t q = next_prime(p);  /* prime immediately following p */

        /* Same number of decimal digits? */
        if (num_digits0(p) == num_digits0(q))
        {
            /* Compute digit dot product */
            seq_t dot = 0;
            uint64_t ap = p, aq = q;
            while (ap && aq)
            {
                uint8_t da = ap % 10;
                uint8_t db = aq % 10;
                dot += (seq_t)(da * db);
                ap /= 10;
                aq /= 10;
            }

            /* dot is prime? */
            if (is_prime_big(dot))
            {
                /* Grow cache if necessary */
                if (terms_len == terms_cap)
                {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                    seq_t *new_terms = mem_resize(seq_t, terms, new_cap);
                    if (!new_terms)   /* allocation failure – return 0 (unlikely) */
                        return 0;
                    terms = new_terms;
                    terms_cap = new_cap;
                }
                terms[terms_len++] = p;   /* store term */
            }
        }

        /* move candidate beyond the current prime for next iteration */
        ++candidate;
    }

    return terms[n - 1];
}
