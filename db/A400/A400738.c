/*
 * A400738  For n >= 1, a(n) is the smallest twin prime representable as the
 *          sum of 2*n - 1 consecutive terms of A077800.
 * https://oeis.org/A400738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) is the smallest twin prime that can be represented * as the
 *            sum of 2*n-1 consecutive terms of A077800. * The calculation
 *            proceeds by generating the sequence * S of primes occurring in
 *            at least one twin pair (each * twin pair contributes its two
 *            members, allowing a prime to * appear twice if...
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
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

/* --------------------------------------------------------------------- */
/*  Global state for the generating sequence S (primes in a twin pair)  */
static seq_t *S = NULL;       /* array holding the terms   */
static size_t S_size = 0;      /* number of elements stored */
static size_t S_cap  = 0;      /* allocated capacity        */

/* Prime generation: yields successive odd primes >=3 via Miller–Rabin. */
static uint64_t prime_candidate = 3;   /* next odd number to test */

/* Return next prime >= prime_candidate and advance candidate. */
static uint64_t next_prime(void)
{
    for (;;) {
        if (is_prime_big((seq_t)prime_candidate)) {
            uint64_t p = prime_candidate;
            prime_candidate += 2;          /* skip even numbers */
            return p;
        }
        prime_candidate += 2;
    }
}

/* Ensure that S has at least 'needed' elements.  If not, generate more
 * terms by scanning primes p and adding the twin pair (p,p+2) whenever
 * both are prime.  The duplicated primes that belong to two pairs are
 * automatically inserted twice because the loop visits each prime once. */
static void ensure_S_size(size_t needed)
{
    while (S_size < needed) {
        uint64_t p = next_prime();               /* next prime p        */
        if (is_prime_big((seq_t)p + 2)) {         /* forms a twin pair?  */
            /* grow if necessary */
            if (S_size + 2 > S_cap) {
                size_t new_cap = S_cap ? S_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, S, new_cap);
                if (!tmp) exit(EXIT_FAILURE);
                S      = tmp;
                S_cap  = new_cap;
            }
            S[S_size++] = (seq_t)p;
            S[S_size++] = (seq_t)(p + 2);
        }
        /* otherwise ignore, just move on to next prime */
    }
}

/* Helper: test whether v is a twin prime (v or v±2 is part of a twin). */
static bool is_twin_prime(seq_t v)
{
    if (!is_prime_big(v))
        return false;
    if (v >= 3 && is_prime_big(v - 2))
        return true;
    if (is_prime_big(v + 2))
        return true;
    return false;
}

/* Main sequence function.  n is a 1‑based index. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* out of bounds – undefined */
    idx_t L = 2 * n - 1;                    /* window length   */

    /* Make sure we have at least L elements (for the first window). */
    ensure_S_size((size_t)L);

    /* Compute the sum of the first window. */
    seq_t sum = 0;
    for (idx_t i = 0; i < L; ++i)
        sum += S[i];

    /* Check if it is a twin prime. */
    if (is_twin_prime(sum))
        return sum;

    /* Slide the window until the first twin prime sum appears.
     * Because the sequence S is non‑decreasing, sums are monotone
     * non‑decreasing, so the first such sum is the smallest. */
    idx_t start = 1;                       /* start index of window */
    for (;;) {
        /* Ensure that the element needed for the new window exists. */
        ensure_S_size((size_t)(start + L));

        /* Update running sum: remove leftmost, add new rightmost. */
        sum -= S[start - 1];
        sum += S[start + L - 1];

        if (is_twin_prime(sum))
            return sum;
        ++start;
    }
}
