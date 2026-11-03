/*
 * A359946  Lexicographically earliest sequence of distinct positive integers
 *          such that for any n > 0, n XOR a(n) is a prime number (where XOR
 *          denotes the bitwise XOR operator).
 * https://oeis.org/A359946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the lexicographically earliest self‑inverse permutation
   of the positive integers such that for all n > 0,
   n XOR a(n) is a prime.  The sequence is built greedily: for each
   unassigned n, pick the smallest unused integer m with (n XOR m)
   prime and set a(n)=m, a(m)=n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

/* Ensure that 'arr' has at least 'needed' elements.
   New slots are zero–initialised. */
static void ensure_perm_cap(size_t needed, seq_t **arr, size_t *cap)
{
    if (needed <= *cap)
        return;
    size_t newcap = *cap ? *cap * 2 : 16;
    while (newcap < needed)
        newcap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, newcap);
    if (!tmp)
        abort();                /* memory exhaustion */
    for (size_t i = *cap; i < newcap; ++i)
        tmp[i] = 0;
    *arr = tmp;
    *cap = newcap;
}

static void ensure_used_cap(size_t needed, uint8_t **arr, size_t *cap)
{
    if (needed <= *cap)
        return;
    size_t newcap = *cap ? *cap * 2 : 16;
    while (newcap < needed)
        newcap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(*arr, newcap * sizeof(uint8_t));
    if (!tmp)
        abort();
    for (size_t i = *cap; i < newcap; ++i)
        tmp[i] = 0;
    *arr = tmp;
    *cap = newcap;
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* static state shared between calls */
    static seq_t *perm  = NULL;          /* perm[i] = a(i)  (1‑based) */
    static size_t perm_cap = 0;
    static uint8_t *used  = NULL;        /* used[x]==1 iff x already used */
    static size_t used_cap = 0;

    /* Ensure storage for values up to n */
    if ((size_t)n + 1 > perm_cap)
        ensure_perm_cap((size_t)n + 1, &perm, &perm_cap);
    if ((size_t)n + 1 > used_cap)
        ensure_used_cap((size_t)n + 1, &used, &used_cap);

    /* Greedily assign all still unassigned positions up to n. */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (perm[i] != 0)            /* already assigned */
            continue;

        /* Find the smallest unused integer candidate. */
        size_t cand = 1;
        for (;;) {
            if ((size_t)cand + 1 > used_cap)
                ensure_used_cap((size_t)cand + 1, &used, &used_cap);
            if (!used[cand]) {
                seq_t xored = (seq_t)i ^ (seq_t)cand;
                if (is_prime_big(xored))
                    break;
            }
            ++cand;
        }

        /* Ensure space for the partner index. */
        if ((size_t)cand + 1 > perm_cap)
            ensure_perm_cap((size_t)cand + 1, &perm, &perm_cap);
        if ((size_t)cand + 1 > used_cap)
            ensure_used_cap((size_t)cand + 1, &used, &used_cap);

        /* Assign the pair. */
        perm[i] = (seq_t)cand;
        perm[cand] = (seq_t)i;

        /* Mark both numbers as used. */
        used[i] = 1;
        used[cand] = 1;
    }

    return perm[(size_t)n];
}
