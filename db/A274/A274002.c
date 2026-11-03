/*
 * A274002  Primes p of the form (q-1)^2+1 that are a divisor of 4^(q-1)-1
 *          where q is prime.
 * https://oeis.org/A274002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A274002
 * Computation: Search successive prime q. For each prime q compute
 *     p = (q-1)^2 + 1.
 * If p is prime and 4^(q-1) ≡ 1 (mod p) then p is appended to the
 * sequence.  The sequence is zero-indexed internally; a(1) is the
 * first term 5, etc.
 */

#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Compute the nth term of A274002. */
seq_t a(idx_t n)
{
    /* Zero index into the internal array */
    static seq_t *seq = NULL;
    static size_t size = 0;        /* number of elements stored */
    static size_t cap  = 0;        /* allocated capacity */
    static uint64_t last_q = 2;    /* last prime q processed */

    if (n == 0) {
        return (seq_t)0;           /* undefined, but safe default */
    }

    /* Generate terms until we have at least n entries */
    while (size < (size_t)n) {
        for (uint64_t q = last_q + 1; ; ++q) {
            if (!is_prime(q)) {
                /* q is not prime, continue searching */
                continue;
            }

            /* Compute p = (q-1)^2 + 1 in arbitrary precision */
            seq_t q_minus = (seq_t)(q - 1);
            seq_t p = q_minus * q_minus + (seq_t)1;

            /* p must be prime */
            if (!is_prime_big(p)) {
                continue;
            }

            /* Check 4^(q-1) ≡ 1 (mod p) */
            seq_t r = powmod((seq_t)4, q - 1, p);
            if (r != (seq_t)1) {
                continue;
            }

            /* All conditions satisfied; add to sequence */
            append_candidate(&seq, &size, &cap, p);
            last_q = q;
            break;                /* next term found, break the for-loop */
        }
    }

    /* Return the requested term (1‑based index to 0‑based array) */
    return seq[n - 1];
}
