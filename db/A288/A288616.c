/*
 * A288616  Primes of the form k!6+27, where k!6 is the sextuple factorial
 *          number (A085158).
 * https://oeis.org/A288616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: standard, pass, 16 terms through n = 16 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A288616: primes of the form k!6 + 27, where k!6 is the sextuple
 * factorial (product of all integers ≤k that are congruent to k mod 6).  */
/* This implementation incrementally constructs the sextuple factorial for each
 * k and tests candidate = k!6 + 27 for primality using is_prime_big.
 * Results are memoised across calls so that any n ≥ 1 may be requested in
 * any order. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    /* Memoised array of found primes */
    static seq_t *primes = NULL;
    static size_t prime_count = 0;
    static size_t prime_cap   = 0;

    /* Current sextuple factorial products for residues 0‑5, initialised to 1 */
    static seq_t prod[6] = {1,1,1,1,1,1};

    /* Current k value */
    static idx_t k = 1;

    /* Generate primes until we have at least n of them */
    while (prime_count < n) {
        int r = (int)(k % 6);
        /* Update the product for this residue with the current k */
        prod[r] = prod[r] * (seq_t)k;

        /* Candidate prime */
        seq_t cand = prod[r] + (seq_t)27;

        if (is_prime_big(cand)) {
            /* Resize array if necessary */
            if (prime_count == prime_cap) {
                size_t newcap = prime_cap ? prime_cap * 2 : 1;
                grow_seq(&primes, &prime_cap, newcap);
            }
            primes[prime_count++] = cand;
        }
        ++k;
    }

    return primes[n - 1];
}
